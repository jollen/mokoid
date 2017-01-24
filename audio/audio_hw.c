#define LOG_TAG "audio_hw_primary"
/*#define LOG_NDEBUG 0*/

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <cutils/str_parms.h>
                                                                                   
#include <hardware/audio.h>
#include <hardware/hardware.h>

#include <linux/videodev2.h>

#include <system/audio.h>

#include <tinyalsa/asoundlib.h>

#include <audio_utils/resampler.h>
#include <audio_route/audio_route.h>

int adev_open_output_stream(struct audio_hw_device *dev,
                          audio_io_handle_t handle,
                          audio_devices_t devices,
                          audio_output_flags_t flags,
                          struct audio_config *config,
                          struct audio_stream_out **stream_out)
{
	ALOGI(LOG_TAG, "audio_config::offload_info::sample_rate = %d\n", config->offload_info.sample_rate);
	ALOGI(LOG_TAG, "audio_config::offload_info::format = %d\n", config->offload_info.format);	
	ALOGI(LOG_TAG, "audio_config::offload_info::bit_rate = %d\n", config->offload_info.bit_rate);
	ALOGI(LOG_TAG, "audio_config::offload_info::duration_us = %d\n", config->offload_info.duration_us);
}

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)                                         
{
    struct audio_device *adev;
    int ret;

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    adev = calloc(1, sizeof(struct audio_device));
    if (!adev)
        return -ENOMEM;

    adev->hw_device.open_output_stream = adev_open_output_stream;

    *device = &adev->hw_device.common;                                             

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "Lyra audio HW HAL",
        .author = "The Android Open Source Project",
        .methods = &hal_module_methods,
    },
};
