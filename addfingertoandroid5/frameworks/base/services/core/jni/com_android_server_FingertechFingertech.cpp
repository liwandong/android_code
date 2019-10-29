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

jbyte gimgbuf[192*256];
int fpsfd = 0;

#define FINGERTECH_IOCTL_MAGIC_NO          0xFB
#define FINGERTECH_INIT                    _IOW(FINGERTECH_IOCTL_MAGIC_NO, 0, jbyte)
#define FINGERTECH_GETIMAGE                _IOW(FINGERTECH_IOCTL_MAGIC_NO, 1, jbyte)
#define FINGERTECH_INITERRUPT_MODE         _IOW(FINGERTECH_IOCTL_MAGIC_NO, 2, jbyte)
#define FINGERTECH_RELEASE_WAKELOCK        _IO (FINGERTECH_IOCTL_MAGIC_NO, 3)
#define FINGERTECH_CHECK_INTERRUPT         _IO (FINGERTECH_IOCTL_MAGIC_NO, 4)
#define FINGERTECH_SET_SPI_SPEED           _IOW(FINGERTECH_IOCTL_MAGIC_NO, 5, jbyte)
#define FINGERTECH_REPORT_KEY              _IOW(FINGERTECH_IOCTL_MAGIC_NO, 6 ,jbyte)
#define FINGERTECH_POWERDOWN               _IO (FINGERTECH_IOCTL_MAGIC_NO, 7)

static const char *fingertechclassname = "com/android/server/FingertechfingerprintService";

//初始化agc
static jint fingertech_setimageagc(JNIEnv *env,jobject obj,jbyte agcval)
{
  jint ret = 0;
  ret = ioctl(fpsfd,FINGERTECH_INIT,agcval);
  return ret;
}

//读图
static jint fingertech_readimgerbuf(JNIEnv *env,jobject obj,jbyteArray buf)
{
 jint ret = 0;
 jbyte *bytearray;
 ret = ioctl(fpsfd,FINGERTECH_GETIMAGE);
 if(ret < 0)
 {
   return ret;
 }
 //jbyteArray->jbyte 指针 -> c uint8_t 数组
 bytearray = env->GetByteArrayElements(buf,NULL);
 if(NULL == bytearray)
 {
  return -1;
 }
 ret = read(fpsfd,bytearray,192*256);
 //c中的数组操作同步到java中，并且释放资源
 env->ReleaseByteArrayElements(buf,bytearray,0);
 return ret;
}

//打开设备
static jint fingertech_opendev(JNIEnv *env,jobject obj)
{
   fpsfd = open("/dev/fpsdev0",O_RDWR);
   ALOGE("%s fd=%d","fingertech_opendev",fpsfd);
   return fpsfd;
}

//关闭设备
static void fingertech_closedev(JNIEnv *env,jobject obj)
{
   close(fpsfd);
}

namespace android {
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
    const JNINativeMethod* gMethods, int numMethods)
{
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

//JNI_Onload 方法
#if 0
jint JNIEXPORT JNI_OnLoad(JavaVM* vm,void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
    if ((*vm)->GetEnv(vm,(void**) &env, JNI_VERSION_1_4) != JNI_OK) {
     return result;
    }
    jniRegisterNativeMethods(env,fingertechclassname,fingertech_Methods_table,sizeof(fingertech_Methods_table)/sizeof(fingertech_Methods_table[0]));
    return JNI_VERSION_1_4;
}
#endif

