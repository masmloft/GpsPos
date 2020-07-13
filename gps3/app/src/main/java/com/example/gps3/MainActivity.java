package com.example.gps3;

import java.util.Date;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.GpsSatellite;
import android.location.GpsStatus;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.core.app.ActivityCompat;

public class MainActivity extends Activity {
    TextView tvEnabledGPS;
    TextView tvStatusGPS;
    TextView tvLocationGPS;
    TextView textViewSatCount;

    private Gps _gps = new Gps(this);

    public class MyClass implements Gps.Callback {
        @Override
        public void emit() {
            showLocation();
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

        _gps.callback = new MyClass();
    }

    @Override
    protected void onResume() {
        super.onResume();

        _gps.start();
    }

    @Override
    protected void onPause() {
        super.onPause();
        _gps.stop();
    }

    public void onClickLocationSettings(View view) {
        startActivity(new Intent(
                android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS));
    };

    public void onClickButtonStart(View view) {
        startService(new Intent(this, GpsService.class));
    };

    public void onClickButtonStop(View view) {
        stopService(new Intent(this, GpsService.class));
    };

    private void showLocation() {
        Gps.Data data = _gps.data();
        String s = String.format(
                "Coordinates: lat = %1$.7f, lon = %2$.7f, time = %3$tF %3$tT",
                data.lat, data.lon, new Date(data.date));
        tvLocationGPS.setText(s);
    }

}