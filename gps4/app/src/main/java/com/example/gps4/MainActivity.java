package com.example.gps4;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.GpsSatellite;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    public static final String LOG_TAG = "MainActivity";
    public final static String BROADCAST_ACTION = "com.example.gps4";

    TextView tvEnabledGPS;
    TextView tvStatusGPS;
    TextView tvLocationGPS;
    TextView textViewSatCount;

    BroadcastReceiver _broadcastReceiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvEnabledGPS = (TextView) findViewById(R.id.tvEnabledGPS);
        //tvStatusGPS = (TextView) findViewById(R.id.tvStatusGPS);
        tvLocationGPS = (TextView) findViewById(R.id.tvLocationGPS);
        textViewSatCount = (TextView) findViewById(R.id.textViewSatCount);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        stopService(new Intent(this, GpsService.class));
        if(_broadcastReceiver != null)
        {
            unregisterReceiver(_broadcastReceiver);
            _broadcastReceiver = null;
        }
    }

    public void onClickButtonStart(View view)
    {
        if(_broadcastReceiver == null)
        {
            stopService(new Intent(this, GpsService.class));
            {
                _broadcastReceiver = new BroadcastReceiver() {
                    public void onReceive(Context context, Intent intent) {
                        GpsData data = (GpsData) intent.getSerializableExtra(GpsService.MSG_DATA);
                        showLocation(data);
                    }
                };
                // создаем фильтр для BroadcastReceiver
                IntentFilter intFilt = new IntentFilter(BROADCAST_ACTION);
                // регистрируем (включаем) BroadcastReceiver
                registerReceiver(_broadcastReceiver, intFilt);
            }
            {
                Intent intent = new Intent(this, GpsService.class);
                //intent.putExtra(PARAM_TASK, TASK1_CODE);
                startService(intent);

            }
        }
        tvEnabledGPS.setText("Enabled: " + (_broadcastReceiver != null));
    };

    public void onClickButtonStop(View view) {
        stopService(new Intent(this, GpsService.class));
        if(_broadcastReceiver != null)
        {
            unregisterReceiver(_broadcastReceiver);
            _broadcastReceiver = null;
        }
        tvEnabledGPS.setText("Enabled: " + (_broadcastReceiver != null));
    };

    private void showLocation(GpsData data)
    {
        Log.d(LOG_TAG, "showLocation");

        tvEnabledGPS.setText("Enabled: " + data.enabledGPS);

        if(data.date == 0)
            Log.d(LOG_TAG, "0");


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
            for (GpsSat sat : data.sats) {
                //if(sat.usedInFix())
                s += String.format("| %d : %f |", i, sat.mSnr) + "\n";
            }

            textViewSatCount.setText(s);
        }
    }

}