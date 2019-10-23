#需要生成的平台
APP_ABI = armeabi-v7a 
#CPP的属性
APP_CPPFLAGS += -fno-rtti
#debug 还是realse
APP_OPTIM = debug
#C++ 标准库
APP_STL := c++_static
#解决ndk本地的版本比要编译的库的ndk版本高
APP_ALLOW_MISSING_DEPS=true
#APP_PLATFORM 会声明编译此应用所面向的 Android API 级别
APP_PLATFORM := android-21
