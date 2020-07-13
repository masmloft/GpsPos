package com.example.gps3;

import java.util.Date;

import android.Manifest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.GpsSatellite;
import android.location.GpsStatus;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.core.app.ActivityCompat;

public class MainActivity extends Activity {
    public static final String TAG = "MainActivity";

    TextView tvEnabledGPS;
    TextView tvStatusGPS;
    TextView tvLocationGPS;
    TextView textViewSatCount;

    private GpsServiceConnection _gpsServiceConnection;
    private GpsService _gpsService;
    private Messenger _gpsServiceMessenger;
    final private Messenger messenger = new Messenger(new IncomingHandler());

    //private Gps _gps = new Gps(this);

    public class GpsCallback implements Gps.Callback {
        @Override
        public void emit() {
            //showLocation(_gps.data());
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvEnabledGPS = (TextView) findViewById(R.id.tvEnabledGPS);
        tvStatusGPS = (TextView) findViewById(R.id.tvStatusGPS);
        tvLocationGPS = (TextView) findViewById(R.id.tvLocationGPS);
        textViewSatCount = (TextView) findViewById(R.id.textViewSatCount);

        //_gpsServiceConnection = new GpsServiceConnection();
//        bindService(new Intent(this, GpsService.class), _gpsServiceConnection, Context.BIND_AUTO_CREATE);
//        _gps.callback = new GpsCallback();
    }

    @Override
    public void onDestroy() {
        if(_gpsServiceConnection != null)
        {
            unbindService(_gpsServiceConnection);
//            stopService(new Intent(this, GpsService.class));
            _gpsServiceConnection = null;
        }
        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
//        bindService(new Intent(this, GpsService.class), _gpsServiceConnection, Context.BIND_AUTO_CREATE);
//        _gps.start();
    }

    @Override
    protected void onPause() {
        super.onPause();
//        unbindService(_gpsServiceConnection);
//        _gps.stop();
    }

    private class GpsServiceConnection implements ServiceConnection {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            _gpsService = ((GpsService.GpsServiceBinder) binder).getService();
            //_gpsServiceMessenger = new Messenger(binder);
            _gpsServiceMessenger = ((GpsService.GpsServiceBinder) binder).getMessanger();
            Message msg = Message.obtain(null, GpsService.GET_DATA);
            msg.replyTo = messenger;
            try {
                _gpsServiceMessenger.send(msg);
            }
            catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {	}
    }

    public void onClickLocationSettings(View view) {
        startActivity(new Intent(
                android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS));
    };

    public void onClickButtonStart(View view) {
        if(_gpsServiceConnection == null) {
//        startService(new Intent(this, GpsService.class));
            _gpsServiceConnection = new GpsServiceConnection();
            bindService(new Intent(this, GpsService.class), _gpsServiceConnection, Context.BIND_AUTO_CREATE);
        }
        tvEnabledGPS.setText("Enabled: " + (_gpsServiceConnection != null));
    };

    public void onClickButtonStop(View view) {
        if(_gpsServiceConnection != null)
        {
            unbindService(_gpsServiceConnection);
            stopService(new Intent(this, GpsService.class));
            _gpsServiceConnection = null;
        }
        tvEnabledGPS.setText("Enabled: " + (_gpsServiceConnection != null));
    };

    private void showLocation(Gps.Data data) {

        tvEnabledGPS.setText("Enabled: " + data.enabledGPS);

        {
            String s = String.format(
                    "Coordinates: lat = %1$.7f, lon = %2$.7f, alt = %3$.1f, time = %4$tF %4$tT",
                    data.lat, data.lon, data.alt, new Date(data.date));
            tvLocationGPS.setText(s);
        }
        {
            String s = "";
            s += String.format("_satellites: %d", data.sats.size()) + "\n";

            int i = 0;
            for (GpsSatellite sat : data.sats) {
                //if(sat.usedInFix())
                s += String.format("| %d : %f |", i, sat.getSnr()) + "\n";
            }

            textViewSatCount.setText(s);
        }
    }

    private class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg){
            switch (msg.what) {
                case GpsService.DATA:
                    Log.d(TAG, "GpsService.DATA");
                    Gps.Data data = _gpsService.gpsData();
                    showLocation(data);
                    break;
            }
        }
    }

}