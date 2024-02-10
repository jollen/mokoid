/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <math.h>
#include <sys/types.h>

#include <utils/SortedVector.h>
#include <utils/KeyedVector.h>
#include <utils/threads.h>
#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/String16.h>
#include <binder/Binder.h>
#include <binder/IServiceManager.h>

#include <mokoid/led.h>

#include "LedService.h"

namespace android {
// ---------------------------------------------------------------------------

/*
 * Binder proxy object implementation
 */
class BpLedService: public BpInterface<ILedService>
{
public:
    BpLedService(const sp<IBinder>& impl)
        : BpInterface<ILedService>(impl)
    {
    }

    virtual int setOn(int led)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ILedService::getInterfaceDescriptor());
        data.writeInt32(led);
        remote()->transact(BnLedService::LED_ON, data, &reply);
        return 0;
    }

    virtual int setOff(int led, int delay)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ILedService::getInterfaceDescriptor());
        // Parcel.h
        data.writeInt32(led);
        data.writeInt32(delay);
        remote()->transact(BnLedService::LED_OFF, data, &reply);
        return 0;
    }

    virtual int setName(const char *name)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ILedService::getInterfaceDescriptor());
        // writeCString(const char* str);
        data.writeCString(name);
        remote()->transact(BnLedService::LED_SET_NAME, data, &reply);
        return 0;
    }

    virtual int connect(const sp<ILedClient>& ledClient)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ILedClient::getInterfaceDescriptor());
        data.writeStrongBinder(ledClient->asBinder());
        remote()->transact(BnLedService::CONNECT, data, &reply);
        return 0;
    }      
};

IMPLEMENT_META_INTERFACE(LedService, "mokoid.hardware.ILedService");

status_t BnLedService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    int led;
    int delay;

    switch(code) {
        case CONNECT: {
            CHECK_INTERFACE(ILedService, data, reply);
            return NO_ERROR;
        } break;
	case LED_ON:
        CHECK_INTERFACE(ILedService, data, reply);
        led = data.readInt32();
        setOn(led);
	    return NO_ERROR;
    case LED_OFF:
        CHECK_INTERFACE(ILedService, data, reply);
        led = data.readInt32();
        delay = data.readInt32();
        return NO_ERROR;        
    case LED_SET_NAME:
        CHECK_INTERFACE(ILedService, data, reply);
        name = data.readCString();
        setName(name);
        return NO_ERROR;
    default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

/****** Implementation *****/

static struct led_control_device_t *sLedDevice = NULL;

LedService::LedService()
{
    led_module_t* module;

    ALOGI("ledserver->LedService: trying to get stub object");

    if (hw_get_module(LED_HARDWARE_MODULE_ID, (const hw_module_t**)&module) == 0) {
        ALOGI("ledserver->LedService: stub found.");
        if (led_control_open(&module->common, &sLedDevice) == 0) {
            ALOGI("ledserver->LedService: ops not found.");
            return;
        }
    }

    ALOGE("ledserver->LedService: failed to get LED stub");
    return;
}

LedService::~LedService()
{
}
//Singleton
void LedService::instantiate() {
     defaultServiceManager()->addService(
             String16("mokoid.led"), new LedService());
}

int LedService::setOn(int led)
{
    ALOGI("ledserver->LedService::setOn");

    if (sLedDevice == NULL) {
        ALOGI("ledserver: sLedDevice was not fetched correctly.");
        return -1;
    } else {
        return sLedDevice->set_on(sLedDevice, led);
    }

    return 0;
}

int LedService::setOff(int led, int delay)
{
    ALOGI("ledserver->LedService::setOff");

    return 0;
}

int LedService::setContext()
{   
    // TODO: int mNativeContext
    return 0;
}

int LedService::setName(const char *name)
{
    LOGI("LedService JNI: mokoid_setName() is invoked.");

    if (sLedDevice == NULL) {
        LOGI("LedService JNI: sLedDevice was not fetched correctly.");
        return -1;
    } else {
        return sLedDevice->set_name(sLedDevice, name);
    }

    return 0;
}

int LedService::connect(const sp<ILedClient>& ledClient) {

    int callingPid = getCallingPid();

    sp<Client> client;
    client = new Client(ledClient, callingPid);

    //mClients[callingPid] = client;
    //ledClient->notifyCallback();  
    //ledClient>asBinder()->linkToDeath(new myNotifier());

    if (mClient[callingPid] != 0) {
        client = mClient[callingPid].promote();
        if (client != 0) {
            return client;
        }
        mClient[callingPid].clear();
    }

    ALOGI("LedService::connect E (pid %d)", callingPid);
    return 0;
}

LedService::Client::Client(const sp<ILedClient>& ledClient, int clientPid) {
    int callingPid = getCallingPid();
    ALOGI("Client::Client E (pid %d)", callingPid);

    mLedClient = ledClient;
    mClientPid = clientPid;
}

LedService::Client::~Client() {
    ALOGI("Client::~kClient X");
}


// ---------------------------------------------------------------------------



/*
 * Binder proxy object implementation
 */
class BpLedClient: public BpInterface<ILedClient>
{
public:
    BpLedClient(const sp<IBinder>& impl)
        : BpInterface<ILedClient>(impl)
    {
    }

    void notifyCallback(int32_t msgType)
    {
        ALOGV("notifyCallback()");
        Parcel data, reply;
        data.writeInterfaceToken(ILedClient::getInterfaceDescriptor());
        data.writeInt32(msgType);
        remote()->transact(NOTIFY_CALLBACK, data, &reply);
    }
};

IMPLEMENT_META_INTERFACE(LedClient, "mokoid.hardware.ILedClient");

status_t BnLedClient::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
    case NOTIFY_CALLBACK: {
        ALOGV("NOTIFY_CALLBACK");
        CHECK_INTERFACE(ILedClient, data, reply);
        int32_t msgType = data.readInt32();
        notifyCallback(msgType);
        return NO_ERROR;
    } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

// ---------------------------------------------------------------------------
}; // namespace android


