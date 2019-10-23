package com.fingertech.displayfingerprint;

public class Fingerprintjni {
    //声明JNI方法
    public native int fingerprint_open();
    public native void fingerprint_close();
    public native int  fingerprint_getimg(byte[] imgpixbuf);
    public native int  fingerprint_setimgagc(byte agc);
    //静态初始化块
    static {
        System.loadLibrary("fingerprint");
    }

}
