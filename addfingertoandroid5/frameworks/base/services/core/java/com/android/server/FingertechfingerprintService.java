/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.server;

import android.app.AppOpsManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.hardware.input.InputManager;
import android.os.BatteryStats;
import android.os.Handler;
import android.os.IFingertechFingerprint;
import android.os.PowerManager;
import android.os.PowerManagerInternal;
import android.os.Process;
import android.os.RemoteException;
import android.os.IBinder;
import android.os.Binder;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.os.UserHandle;
import android.os.Vibrator;
import android.os.WorkSource;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.util.Slog;
import android.view.InputDevice;
import android.media.AudioAttributes;

import com.android.internal.app.IAppOpsService;
import com.android.internal.app.IBatteryStats;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.ListIterator;

public class FingertechfingerprintService extends IFingertechFingerprint.Stub{
    private static final String TAG = "FingertechFingerorintService";
    private static final boolean DEBUG = false;
    //构造函数
    FingertechfingerprintService(){
       fingerprint_open();
    }
    public int fingertech_setimgagc(byte agc) throws android.os.RemoteException{
      fingerprint_setimgagc(agc);
      return 0;
    }
    public int fingertech_getimgbuf(byte[] imgbuf) throws android.os.RemoteException{
     fingerprint_getimg(imgbuf); 
      return 0;
    }
    public native int fingerprint_getimg(byte[] imgbuf); 
    public native int fingerprint_setimgagc(byte agc);
    public native int fingerprint_open();
    public native void fingerprint_close();
}
