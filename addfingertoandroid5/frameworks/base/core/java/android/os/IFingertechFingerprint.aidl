package android.os;
interface IFingertechFingerprint{
  int fingertech_setimgagc(byte agc);
  int fingertech_getimgbuf(inout byte[] imgbuf);
}
