#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <jni.h>
#include <cutils/log.h>
#include <android_runtime/AndroidRuntime.h>
#include <hardware/hardware.h>
#include <hardware/fingertech_fingerprint.h>
namespace android {
        uint8_t gimgbuf[192*256];
        uint32_t i = 0;
	fingertech_deivce_t *fingertech_device;
	fingertech_module_t *fingertech_module;
	static const char *fingertechclassname = "com/android/server/FingertechfingerprintService";
	//初始化agc
	static JNIEXPORT jint fingertech_setimageagc(JNIEnv *env,jobject obj,jbyte agcval){
  		jint ret = 0;
		ALOGE("fingertech fingertech_setimageagc");
		fingertech_device->fingertech_setimgagc(agcval);
  	 	return ret;
	}
	//读图
	static JNIEXPORT jint JNICALL fingertech_readimgerbuf(JNIEnv *env,jobject obj,jbyteArray buf){
		 jint ret = 0;
		 jbyte *bytearray;
		 ALOGE("fingertech fingertech_readimgerbuf");
		 //读图
		 fingertech_device->fingertech_getimgbuf(gimgbuf);

		 ALOGE("fingertech fingertech_readimgerbuf %x %x %x %x %x",gimgbuf[0],gimgbuf[1],gimgbuf[2],gimgbuf[3],gimgbuf[4]);
		 //获取指针
		 bytearray = env->GetByteArrayElements(buf, NULL);
		 for(i=0;i<192*256;i++)
		 {
		   bytearray[i] = (jbyte)gimgbuf[i];
		   //bytearray[i] = 0x80;
		 }
		 env->ReleaseByteArrayElements(buf,bytearray,0);
		 return ret;
	}

	//打开设备
	static JNIEXPORT jint fingertech_opendev(JNIEnv *env,jobject obj){
	   int err = 0;
	   hw_module_t *hw_module = NULL;
	   hw_device_t *device = NULL;
	   ALOGE("fingertech fingertech_opendev");
	   //获取fingertech_fingerprint模块
	    if (0 != (err = hw_get_module(FINGERTECHFINGERPRINT_HARDWARE_MODULE_ID, (hw_module_t const**)&hw_module))) {
		ALOGE("Can't open fingerprint HW Module, error: %d", err);
		return 0;
	    }   
	    if (NULL == hw_module) {
		ALOGE("No valid fingerprint module");
		return 0;
	    }   
	   fingertech_module = reinterpret_cast<fingertech_module_t*>(hw_module);
	  //获取设备结构体
	  if (0 != (err = fingertech_module->common.methods->open(hw_module, NULL, &device))) {
		ALOGE("Can't open fingerprint methods, error: %d", err);
        	return 0;
    	   }
	   fingertech_device = reinterpret_cast<fingertech_deivce_t*>(device); 
	   return 0;
       }	 


	//关闭设备
	static JNIEXPORT void fingertech_closedev(JNIEnv *env,jobject obj){
            //fingertech_module->common.methods->close();
	}

	//JNI 表
	//java 中的名称  变量签名（参数）返回值 c 中的方法名称
	static JNINativeMethod fingertech_Methods_table[] = {
   	 {"fingerprint_open","()I",(void*)fingertech_opendev},
    	 {"fingerprint_close","()V",(void*)fingertech_closedev},
   	 {"fingerprint_getimg","([B)I",(void*)fingertech_readimgerbuf},
   	 {"fingerprint_setimgagc","(B)I",(void*)fingertech_setimageagc}
	};

	//jniRegisterNativeMethods
	static JNIEXPORT jint jniRegisterNativeMethods(JNIEnv* env, const char* className,
   	 const JNINativeMethod* gMethods, int numMethods){
    		jclass clazz;
    		clazz = env->FindClass(className);
    		if (clazz == NULL) {
        		return -1;
    		}

    	int result = 0;
    	if (env->RegisterNatives(clazz,gMethods, numMethods) < 0) {
        	result = -1;
    	}

    	env->DeleteLocalRef(clazz);
    	return result;
	}
	int register_android_server_FingertechFingerprint(JNIEnv* env){
		 return     jniRegisterNativeMethods(env,fingertechclassname,fingertech_Methods_table,sizeof(fingertech_Methods_table)/sizeof(fingertech_Methods_table[0]));
 	}
}


