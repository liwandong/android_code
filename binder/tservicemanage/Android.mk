LOCAL_PATH:= $(call my-dir)

svc_c_flags =	-Wall -Wextra 
svc_c_flags += -DBINDER_IPC_32BIT=1

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := bctest.c binder.c
LOCAL_CFLAGS += $(svc_c_flags)
LOCAL_MODULE := servicetest
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog libselinux
LOCAL_SRC_FILES := service_manager.c binder.c
LOCAL_CFLAGS += $(svc_c_flags)
LOCAL_MODULE := servicemanager
include $(BUILD_EXECUTABLE)
