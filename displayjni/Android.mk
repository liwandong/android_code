#LOCAL_PATH 表示源码树的路径
LOCAL_PATH := $(call my-dir)
##hw 层编译
#include $(CLEAR_VARS)
#LOCAL_MODULE := fingerprint.default
#LOCAL_MODULE_RELATIVE_PATH := hw
#LOCAL_SRC_FILES := ./fingertech_hal/fingertech_fingerprint.c
#LOCAL_SHARED_LIBRARIES = liblog
#LOCAL_LDLIBS    := -lm -llog 
#LOCAL_MODULE_TAGS := optional
#LOCAL_C_INCLUDES = ./platform/inc/system/core/include \
#                   ./platform/hardware/libhardware/include \
#		   ./fingertech_hal 
#include $(BUILD_SHARED_LIBRARY)




#表示清除部分LOCAL*的值
include $(CLEAR_VARS)
#生产模块的名称
LOCAL_MODULE := fingerprint
LOCAL_SHARED_LIBRARIES:= libcutils liblog
#LOCAL_MODULE_TAGS := optional
#LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_PROGUARD_ENABLED:= disabled
#添加头文件
LOCAL_C_INCLUDES = ./platform/inc/system/core/include \
                   ./platform/hardware/libhardware/include \
                   ./inc \
				   ./platform/external/libpng 
#添加编译的flags
LOCAL_CFLAGS= -Werror
#LOCAL_HAL_STATIC_LIBRARIES := libhealthd
#需要使用的库
LOCAL_LDLIBS=-lm -llog
#使用其他链接库标记
#LOCAL_LDFLAGS
#模块的源文件
LOCAL_SRC_FILES = fingertechjni/fingertech_jni.c
#生成动态库
include $(BUILD_SHARED_LIBRARY)

#生成fingerprintd
#include $(CLEAR_VARS)
#
#LOCAL_MODULE := fingerprintd
#添加头文件
#LOCAL_C_INCLUDES = ./platform/inc/system/core/include \
#                   ./platform/hardware/libhardware/include \
#                   ./inc \
#		  		   ./platform/external/libpng \
#                  ./platform/frameworks/native/include \
#		          ./platform/system/core/include  \
#		          ./platform/system/security/keystore/include 
#
LOCAL_CFLAGS =   -Werror  
##LOCAL_SHARED_LIBRARIES +=  libbinder \
#	        			liblog \
#		    			libhardware \
#	        			libutils \
#						libkeystore_binder
#LOCAL_LDLIBS =  ./fingertechlibs/libbinder.so \
#	       	    ./fingertechlibs/liblog.so \
#		        ./fingertechlibs/libhardware.so \
#	            ./fingertechlibs/libutils.so \
#		        ./fingertechlibs/libkeystore_binder.so
#LOCAL_SRC_FILES :=   \
#				  ./fingerprintd/FingerprintDaemonProxy.cpp \
#				  ./fingerprintd/IFingerprintDaemon.cpp \
#				  ./fingerprintd/IFingerprintDaemonCallback.cpp \
#				  ./fingerprintd/fingerprintd.cpp \
#
#include $(BUILD_EXECUTABLE)

