package com.example.gps3;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.os.Process;
import android.util.Log;

public class GpsService extends Service {
    private final String LOG_TAG = "GpsService";
    public static final int GET_DATA = 1;
    public static final int DATA = 3;

    public class GpsServiceBinder extends Binder {
        GpsService getService() {
            return GpsService.this;
        }
        public Messenger getMessanger() {
            return new Messenger(_messanger.getBinder());
        }
    }

    private IBinder binder = new GpsServiceBinder();
    private Gps _gps = new Gps(this);

    IncomingHandler _inHandler;
    private Messenger _messanger;
    private Messenger _toActivityMessenger;

    ///////////////////////////////////////////////////////////////
    public GpsService() {
    }

    public Gps.Data gpsData() { return _gps.data(); }

    public void onCreate() {
        super.onCreate();
        Log.d(LOG_TAG, "onCreate");
        HandlerThread thread = new HandlerThread("ServiceStartArguments", Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();
        _inHandler = new IncomingHandler(thread.getLooper());
        _messanger = new Messenger(_inHandler);

        _gps.callback = new GpsCallback();
        _gps.start();
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(LOG_TAG, "onStartCommand");
        return super.onStartCommand(intent, flags, startId);
    }

    public void onDestroy() {
        super.onDestroy();
        Log.d(LOG_TAG, "onDestroy");
        _gps.stop();
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(LOG_TAG, "onBind");
//        return _messanger.getBinder();
        return binder;
    }

    private class GpsCallback implements Gps.Callback {
        @Override
        public void emit() {
            sendDataToActivity();
        }
    }

    private class IncomingHandler extends Handler {
        public IncomingHandler(Looper looper){
            super(looper);
        }

        @Override
        public void handleMessage(Message msg){
            //super.handleMessage(msg);

            _toActivityMessenger = msg.replyTo;

            switch (msg.what) {
                case GET_DATA:
                    sendDataToActivity();
                    break;
            }
        }
    }

    void sendDataToActivity()
    {
        if(_toActivityMessenger == null)
            return;
        Message outMsg = Message.obtain(_inHandler, DATA);
        //outMsg.arg1 = _gps.data();
        outMsg.replyTo = _messanger;

        try {
            _toActivityMessenger.send(outMsg);
        }
        catch (RemoteException e) {
            e.printStackTrace();
        }
    }

}
