#define LOG_TAG "android.hardware.led@1.0-service"

#include <android/hardware/led/1.0/ILed.h>
#include <hidl/LegacySupport.h>
#include "ledImpl.h"
using android::hardware::led::V1_0::ILed;
using android::hardware::led::V1_0::implementation::ledImpl;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;


int main() {
#if PASSTHROUGH
    return defaultPassthroughServiceImplementation<Iled>(); 
#else
	// Binderized HAL
	sp<ILed> service = new ledImpl();
	configureRpcThreadpool(1, true);
    if(android::OK !=  service->registerAsService())
			return 1;
    joinRpcThreadpool();
#endif
}





