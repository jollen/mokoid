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
    }
    return mLedService;
}

static jboolean ledmanager_init(JNIEnv *env, jclass clazz)
{
    mLedService = getLedService();
}

static jboolean ledmanager_setName(JNIEnv* env, jobject thiz, jstring ns) {
}

static const JNINativeMethod gMethods[] = {
    {"_init",	  	"()Z",
			(void*)ledmanager_init},
    { "_set_name",     "(Ljava/lang/String;)Z",
                        (void*)ledmanager_setName }            
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
