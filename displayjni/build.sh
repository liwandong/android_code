#!/bin/bash
ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=./Application.mk APP_BUILD_SCRIPT=./Android.mk clean
ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=./Application.mk APP_BUILD_SCRIPT=./Android.mk
#ndk-build
adb remount
adb push $(pwd)/libs/armeabi-v7a/libfingerprint.so /system/lib
#adb shell "stop surfaceflinger" 
#adb shell 
