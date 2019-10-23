#ifndef FINGERTECH_FINGERPRINT_H
#define FINGERTECH_FINGERPRINT_H

#include <hardware/hw_auth_token.h>

//模块ID
#define  FINGERPRINT_HARDWARE_MODULE_ID  "fingertech_fingerprint"

#define FINGERPRINT_MODULE_API_VERSION_1_0 HARDWARE_MODULE_API_VERSION(1, 0)
#define FINGERPRINT_MODULE_API_VERSION_2_0 HARDWARE_MODULE_API_VERSION(2, 0)
#define FINGERPRINT_MODULE_API_VERSION_2_1 HARDWARE_MODULE_API_VERSION(2, 1)

typedef struct fingertech_deivce_t {
   struct hw_device_t common_device;
   //需要实现的函数指
   int (*fingertech_getimgbuf)(uint8_t*buf);
}fingertech_deivce_t; 

typedef struct fingertech_module_t {
   struct hw_module_t common;
}fingertech_module_t;



#endif


