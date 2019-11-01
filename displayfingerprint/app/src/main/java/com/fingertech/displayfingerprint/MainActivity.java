package com.fingertech.displayfingerprint;

import androidx.appcompat.app.AppCompatActivity;
import androidx.multidex.MultiDex;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
//import android.graphics.Bitmap;
import android.widget.Toast;

import java.util.Arrays;
import android.os.IFingertechFingerprint;
import android.os.ServiceManager;


public class MainActivity extends AppCompatActivity {
    private Button mbutton =null;
    private ImageView imageView = null;
    private Bitmap mbitmap = null;
    final int IMG_WIDTH = 192;
    final int IMG_HIGHT = 256;
    private IFingertechFingerprint fingerprintserver=null;
    private EditText editText=null;
    int ret  = 0;
    private String fingeragcstr;
    private byte []imgbuf = new byte[IMG_WIDTH*IMG_HIGHT];

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        MultiDex.install(this);
    }

    //将数据组合成bmp格式的数据
    private byte[] fingertech_newbitmap(byte []pixbuf,int w,int h)
    {
         int bmpsize = 0;
         int count = 0;
         int mw = (w+3)&0xfc;
         int mh = h;

         //bmpsize = 14 + 40 + 256*4 + w*h;
         bmpsize = 14 + 40 + 256*4 + mw*mh;
         byte []bitmapbuf = new byte[bmpsize];
         Arrays.fill(bitmapbuf,(byte)0x00);

         bitmapbuf[0] = 0x42;
         bitmapbuf[1] = 0x4d;
         bitmapbuf[2] = (byte) (bmpsize & 0x000000ff);
         bitmapbuf[3] = (byte) ((bmpsize & 0x0000ff00)>>8);
         bitmapbuf[4] = (byte) ((bmpsize & 0x00ff0000)>>16);
         bitmapbuf[5] = (byte) ((bmpsize & 0xff000000)>>24);
         bitmapbuf[6] = 0x00;
         bitmapbuf[7] = 0x00;
         bitmapbuf[8] = 0x00;
         bitmapbuf[9] = 0x00;
         //文件头开始到实际数据之间的字节的偏移
         //14+40+256*4
         bitmapbuf[10] = 0x36;
         bitmapbuf[11] = 0x04;
         bitmapbuf[12] = 0x00;
         bitmapbuf[13] = 0x00;
         //位图信息头大小
         bitmapbuf[14] = 0x28;
         bitmapbuf[15] = 0x00;
         bitmapbuf[16] = 0x00;
         bitmapbuf[17] = 0x00;
         //图像的宽度
         bitmapbuf[18] = (byte) (mw & 0xff);
         bitmapbuf[19] = (byte) ((mw>>8) & 0xff);
         bitmapbuf[20] = (byte) ((mw>>16)& 0xff);
         bitmapbuf[21] = (byte) ((mw>>24) & 0xff);
         //图像高度
         bitmapbuf[22] = (byte) (mh & 0xff);
         bitmapbuf[23] = (byte) ((mh>>8) & 0xff);
         bitmapbuf[24] = (byte) ((mh>>16)& 0xff);
         bitmapbuf[25] = (byte) ((mh>>24) & 0xff);

         bitmapbuf[26] = 0x01;
         bitmapbuf[27] = 0x00;
         bitmapbuf[28] = 0x08;
         bitmapbuf[29] = 0x00;

         for(count=0;count<256;count++)
         {
             bitmapbuf[54+count*4] = (byte) count;
             bitmapbuf[54+count*4+1] = (byte) count;
             bitmapbuf[54+count*4+2] = (byte) count;
         }
         for(count = 0;count<w*h;count++)
         {
             bitmapbuf[1078+count] = pixbuf[count];
         }
         return  bitmapbuf;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //输入框
        editText = (EditText)findViewById(R.id.myedittext);
        //找到button
        mbutton = (Button) findViewById(R.id.mybutton);
        //找到显示图片的控件
        imageView = (ImageView)findViewById(R.id.myimageview);

        IBinder mybinder = ServiceManager.getService("myfingerprint");
        if( mybinder== null)
        {
            Toast.makeText(getApplicationContext(),"获取指纹服务失败",Toast.LENGTH_SHORT).show();
        }

        //创建jni实例
        fingerprintserver = IFingertechFingerprint.Stub.asInterface(mybinder);
        if(null == fingerprintserver)
        {
            Toast.makeText(getApplicationContext(),"获取指纹服务失败",Toast.LENGTH_SHORT).show();
        }
       // editText.setInputType(InputType.TYPE_NUMBER_VARIATION_NORMAL);
        //设置监听器
        mbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                fingeragcstr = editText.getText().toString();
                Log.d("fingerprint",fingeragcstr);
                if(Integer.parseInt(fingeragcstr)>255) {
                    Toast.makeText(v.getContext(),"最大值不能超过255",Toast.LENGTH_SHORT).show();
                }
                else{
                    //设置agc
                    try {
                        ret = fingerprintserver.fingertech_setimgagc((byte)Integer.parseInt(fingeragcstr));
                        if(ret <0)
                        {
                            Toast.makeText(v.getContext(),"配置agc失败",Toast.LENGTH_SHORT).show();
                        }
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                    //读图
                    try {
                        ret = fingerprintserver.fingertech_getimgbuf(imgbuf);
                        Log.d("fingerprint vvvvvvvv",imgbuf.toString());
                        if(ret <0)
                        {
                            Toast.makeText(v.getContext(),"获取图像失败",Toast.LENGTH_SHORT).show();
                        }
                        else {
                            //Arrays.fill(imgbuf,(byte)0xff);
                            byte[] testbuf = fingertech_newbitmap(imgbuf, IMG_WIDTH, IMG_HIGHT);

                            mbitmap = BitmapFactory.decodeByteArray(testbuf, 0, testbuf.length);
                            imageView.setImageBitmap(mbitmap);
                        }

                    }catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
    }
}
