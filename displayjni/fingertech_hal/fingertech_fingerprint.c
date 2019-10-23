#include <stdio.h>
#include <hardware/hardware.h>
#include <string.h>
#include "fingertech_fingerprint.h"




static int fingertech_fingerprintclose(hw_device_t *dev)
{
	if (dev)
	{
	 free(dev);
	}
	return 0;
}

static int fingertech_getfingerprintimage(uint8_t* pixbuf)
{

        return 0;
}

static int fingertech_fingerprintopen(const struct hw_module_t* module, const char* id,struct hw_device_t** device)
{
	fingertech_deivce_t *dev;
	dev = malloc(sizeof(fingertech_deivce_t));
	memset(dev,0,sizeof(fingertech_deivce_t));
	dev->common_device.tag = HARDWARE_DEVICE_TAG;
	dev->common_device.version = FINGERPRINT_MODULE_API_VERSION_2_0;
	dev->common_device.module = (struct hw_module_t*) module;
	dev->common_device.close = fingertech_fingerprintclose;
        dev->fingertech_getimgbuf = fingertech_getfingerprintimage;
	*device = (hw_device_t*) dev;
	return 0;
}


struct hw_module_methods_t fingertech_module = {
   .open = fingertech_fingerprintopen, 
};




struct fingertech_module_t HAL_MODULE_INFO_SYM = {
    .common = {
         .tag = HARDWARE_MODULE_TAG,
	 .module_api_version = FINGERPRINT_MODULE_API_VERSION_2_0,
	 .hal_api_version = HARDWARE_HAL_API_VERSION,
	 .id = FINGERPRINT_HARDWARE_MODULE_ID,
	 .name = "fingertech fingerprint",
	 .author  = "wendell",
	 .methods = &fingertech_module,
	},
};

