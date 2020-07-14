package com.example.gps4;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;

import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class GpsService extends Service {
    public static final String LOG_TAG = "GpsService";
    public final static String MSG_DATA = "MSG_GPSDATA";

    private Gps _gps;

    //ExecutorService es;
////////////////////////////////////////////////////////////////////////////////////////
    public GpsService() {
    }

    public void onCreate() {
        super.onCreate();
        Log.d(LOG_TAG, "onCreate");
        //es = Executors.newFixedThreadPool(1);
        if(_gps == null) {
            _gps = new Gps(this);
            _gps.callback = new GpsCallback();
            _gps.start();
        }
    }

    public void onDestroy() {
        super.onDestroy();
        Log.d(LOG_TAG, "onDestroy");
        if(_gps != null) {
            _gps.stop();
            _gps = null;
        }
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

    public int onStartCommand(Intent intent, int flags, int startId)
    {
        Log.d(LOG_TAG, "onStartCommand");

/*
        int time = intent.getIntExtra(MainActivity.PARAM_TIME, 1);
        int task = intent.getIntExtra(MainActivity.PARAM_TASK, 0);

        MyRun mr = new MyRun(startId, time, task);
        es.execute(mr);
*/

//        sendDataToActivity();

        return super.onStartCommand(intent, flags, startId);
    }

    private class GpsCallback implements Gps.Callback {
        @Override
        public void emit() {
            sendDataToActivity();
        }
    }

    void sendDataToActivity()
    {
        if(_gps != null) {

            Intent intent = new Intent(MainActivity.BROADCAST_ACTION);
            Log.d(LOG_TAG, "sendDataToActivity");
            intent.putExtra(GpsService.MSG_DATA, _gps.data());
            if (_gps.data().date == 0)
                Log.d(LOG_TAG, "0");

            sendBroadcast(intent);
        }
    }

}
