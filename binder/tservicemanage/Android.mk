LOCAL_PATH:= $(call my-dir)

svc_c_flags =	-Wall -Wextra 
svc_c_flags += -DBINDER_IPC_32BIT=1

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_LDLIBS    := -lm -llog
LOCAL_SRC_FILES := bctest.c binder.c
LOCAL_C_INCLUDES := ./inc
LOCAL_CFLAGS += $(svc_c_flags)
LOCAL_MODULE := servicetest
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog libselinux
LOCAL_LDLIBS    := ./lib/liblog.so ./lib/libselinux.so
LOCAL_SRC_FILES := service_manager.c binder.c
LOCAL_STATIC_LIBRARIES := libmincrypt
LOCAL_C_INCLUDES := ./inc
LOCAL_CFLAGS += $(svc_c_flags) -DAUDITD_LOG_TAG=1003
LOCAL_MODULE := servicemanager
include $(BUILD_EXECUTABLE)
