#define ALOG_TAG "MokoidPlatform"
#include "utils/Log.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <jni.h>
#include <mokoid/led.h>

#include "LedService.h"

using namespace android;

struct led_control_device_t *sLedDevice = NULL;

sp<ILedService> mLedService;

const sp<ILedService>& getLedService()
{
    LOGI("getLedService()");

    //TODO need Mutex
    //Mutex::Autolock _l(mLock);

    if (mLedService.get() == 0) {
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;
        do {
            binder = sm->getService(String16("mokoid.led"));
            if (binder != 0)
                break;
            LOGI("LedService not published, waiting...");
            usleep(100000); // 0.1s is better
        } while(true);

        mLedService = ILedService::asInterface(binder);

        sp<LedClient> s = new LedClient();
        mLedService.connect(s);
    }
    return mLedService;
}

static jboolean ledmanager_init(JNIEnv *env, jclass clazz, jboject)
{
    mLedService = getLedService();
}

static jboolean ledmanager_setName(JNIEnv* env, jobject thiz, jstring ns) {
}

static const char* kClassName ="com/mokoid/LedTest";
static const char *delegationField = "mDelegateID";
static const jclass clazz;

static jboolean ledmanager_setOff(JNIEnv* env, jobject thiz, jint led, jint delay) {
    int status = mLedService.setOff(led, delay);

    // get delegation context
    jfieldId fieldDelegateID = env->GetFieldID(clazz, delegationField, "I");
    int id = env->GetIntField(thiz, fieldDelegation);
    ClientDelegate* context = reinterpret_cast<ClientDelegate*>(id);

    // set java application fields
    env->SetIntField(thiz, context->getFieldNativeContext(), status);
    env->SetIntField(thiz, context->getFieldPeriodContext(), 0);

    // callback java application method
    context->mWeakContext->notifyLedChanged();  

    return 0;
}

void ClientDelegate::release()
{
    ALOGV("ClientDelegate::release");

    if (mWeakContext != NULL) {
        mJNIEnv->DeleteGlobalRef(mWeakContext);
        mWeakContext = NULL;
    }
}

ClientDelegate::ClientDelegate(JNIEnv *env, jobject weak_this, jclass clazz)
{
    ALOGV("ClientDelegate::ClientDelegate");

    mJNIEnv = env;
    kClazz = clazz;
    mWeakContext = mJNIEnv->NewGlobalRef(weak_this);
}

// store persistent context for application context
class ClientDelegate: virtual public RefBase
{
public:
    ClientDelegate(JNIEnv *env, jobject weak_this, jclass clazz);
    ~ClientDelegate() { release(); }
    void release();

    // field getters
    jfieldId getFieldNativeContext() { return mJNIEnv->GetFieldID(kClazz, "mNativeContext", "I"); }
    jfieldId getFieldPeriodContext() { return mJNIEnv->GetFieldID(kClazz, "mPeriodContext", "I"); }

private:
    JNIEnv*     mJNIEnv;
    jclass      kClazz;
    jobject     mWeakContext;    // weak reference to java application object
    int         status;          // start custom attributes from here ...
};

static jboolean ledmanager_native_setup(JNIEnv* env, jobject thiz, jobject weak_this) {
    clazz = env->FindClass(kClassName);

    // object reference management
    sp<ClientDelegate> context = new ClientDelegate(env, weak_this, clazz);
    context->incStrong(thiz);

    // set object id
    int id = context.get();
    jfieldId field = env->GetFieldID(clazz, delegationField, "I");
    env->SetIntField(thiz, field, id);
}

static const JNINativeMethod gMethods[] = {
    {"_init",	  	"()Z",
			(void*)ledmanager_init},
    {"_native_setup",       "(Ljava/lang/Object;)V",
            (void*)ledmanager_native_setup},            
    { "_set_name",     "(Ljava/lang/String;)Z",
                        (void*)ledmanager_setName } ,
    { "_set_off",     "(II)Z",
                        (void*)ledmanager_setOff }                              
};

int registerMethods(JNIEnv* env) {
    static const char* const kClassName =
        "mokoid/hardware/LedManager2";
    jclass clazz;

    /* look up the class */
    clazz = env->FindClass(kClassName);
    if (clazz == NULL) {
        ALOGE("Can't find class %s\n", kClassName);
        return -1;
    }

    /* register all the methods */
    if (env->RegisterNatives(clazz, gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
    {
        ALOGE("Failed registering methods for %s\n", kClassName);
        return -1;
    }

    /* fill out the rest of the ID cache */
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
	goto bail;
    }
    assert(env != NULL);

    registerMethods(env);

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}
