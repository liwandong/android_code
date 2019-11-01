#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cutils/log.h>
#include <hardware/hardware.h>
#include <string.h>
#include <hardware/fingertech_fingerprint.h>

#define FINGERTECH_IOCTL_MAGIC_NO          0xFB
#define FINGERTECH_INIT                    _IOW(FINGERTECH_IOCTL_MAGIC_NO, 0, uint8_t)
#define FINGERTECH_GETIMAGE                _IOW(FINGERTECH_IOCTL_MAGIC_NO, 1, uint8_t)
#define FINGERTECH_INITERRUPT_MODE         _IOW(FINGERTECH_IOCTL_MAGIC_NO, 2, uint8_t)
#define FINGERTECH_RELEASE_WAKELOCK        _IO(FINGERTECH_IOCTL_MAGIC_NO, 3)
#define FINGERTECH_CHECK_INTERRUPT         _IO(FINGERTECH_IOCTL_MAGIC_NO, 4)
#define FINGERTECH_SET_SPI_SPEED           _IOW(FINGERTECH_IOCTL_MAGIC_NO, 5, uint8_t)
#define FINGERTECH_REPORT_KEY              _IOW(FINGERTECH_IOCTL_MAGIC_NO, 6 , uint8_t)
#define FINGERTECH_POWERDOWN               _IO(FINGERTECH_IOCTL_MAGIC_NO, 7 )

uint8_t gimgbuf[192*256];
uint16_t count = 0;
int fpsfd = 0;

//打开设备
static int fingertech_openfingerprintdev(void)
{
  fpsfd = open("/dev/fpsdev0",O_RDWR);
  ALOGE("fingertech fpsfd = %d",fpsfd); 
  return 0;
}
//关闭设备
static int fingertech_fingerprintclose(hw_device_t *dev)
{
    close(fpsfd);
	if (dev)
	{
	 free(dev);
	}
	return 0;
}

//获取图像
static int fingertech_getfingerprintimage(uint8_t* pixbuf)
{
    int ret = 0;
    ret = ioctl(fpsfd,FINGERTECH_GETIMAGE,1);
    if(ret!=0)
    {
      return ret;
    }
    ret = read(fpsfd,pixbuf,192*256);
    return 0;
}
//设置agc
static int fingertech_setfingerprintimgagc(uint8_t agc)
{
   int ret = 0;
   ret = ioctl(fpsfd,FINGERTECH_INIT,agc);
   return ret;
}

//打开
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
    	dev->fingertech_setimgagc = fingertech_setfingerprintimgagc;
	*device = (hw_device_t*) dev;
    	//打开设备
    	fingertech_openfingerprintdev();
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
        .id = FINGERTECHFINGERPRINT_HARDWARE_MODULE_ID,
        .name = "fingertech fingerprint",
        .author  = "wendell",
        .methods = &fingertech_module,
	},
};

