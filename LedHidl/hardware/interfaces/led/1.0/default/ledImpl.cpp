#define LOG_TAG "LedService"

#include <log/log.h>
#include "ledImpl.h"

namespace android {
namespace hardware {
namespace led {
namespace V1_0 {
namespace implementation {

ledImpl::ledImpl() {
	state = LedStatus::LED_BAD_VALUE;
	ALOGE("ledImpl Init status:%d", state);
}

Return<void> ledImpl::on() {
	state = LedStatus::LED_ON;
	ALOGE("ledImpl on status:%d", state);
    return Void();
}

Return<int32_t> ledImpl::set(LedStatus val) {
	if(val == LedStatus::LED_OFF || val == LedStatus::LED_ON)
		state = val;
	else
		return -1;
	return 0;
}

ILed* HIDL_FETCH_ILed(const char * /*name*/) {
	ALOGE("ledImpl HIDL_FETCH_ILed ");	
    return new ledImpl();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace led
}  // namespace hardware
}  // namespace android