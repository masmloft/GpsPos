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
//    TextView tvEnabledNet;
//    TextView tvStatusNet;
//    TextView tvLocationNet;
    TextView textViewSatCount;

    private LocationManager locationManager;
    StringBuilder sbGPS = new StringBuilder();
    StringBuilder sbNet = new StringBuilder();

    int _satellites = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvEnabledGPS = (TextView) findViewById(R.id.tvEnabledGPS);
        tvStatusGPS = (TextView) findViewById(R.id.tvStatusGPS);
        tvLocationGPS = (TextView) findViewById(R.id.tvLocationGPS);
//        tvEnabledNet = (TextView) findViewById(R.id.tvEnabledNet);
//        tvStatusNet = (TextView) findViewById(R.id.tvStatusNet);
//        tvLocationNet = (TextView) findViewById(R.id.tvLocationNet);
        textViewSatCount = (TextView) findViewById(R.id.textViewSatCount);

        locationManager = (LocationManager) getSystemService(LOCATION_SERVICE);

    }

//    private static void Log(String message) {
//        if(message != null && message.length() > 0) {
//            //android.util.Log.i(LCAT, message);
//            //org.appcelerator.kroll.common.Log.i("UtilsModule", message);
//        }
//    }

    @Override
    protected void onResume() {
        super.onResume();
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }

        GpsStatus.Listener gpsStatusListener = new GpsStatus.Listener() {
            @Override
            public void onGpsStatusChanged(int event) {
                if (event == GpsStatus.GPS_EVENT_SATELLITE_STATUS || event == GpsStatus.GPS_EVENT_FIRST_FIX) {
                    GpsStatus status = locationManager.getGpsStatus(null);
                    Iterable<GpsSatellite> sats = status.getSatellites();
                    // Check number of satellites in list to determine fix state

                    String s = "";

                    _satellites = 0;
                    int i = 0;
                    for (GpsSatellite sat : sats) {
                        //if(sat.usedInFix())
                        s += String.format("| %d : %f |", i, sat.getSnr()) + "\n";
                        _satellites++;
                    }

                   s = String.format("_satellites: %d", _satellites) + "\n" + s;
                    textViewSatCount.setText(s);
                }
            }
        };

        locationManager.addGpsStatusListener(gpsStatusListener);

        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000 * 1, 0, locationListener);
        //locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 1000 * 10, 10, locationListener);

        checkEnabled();
    }

    @Override
    protected void onPause() {
        super.onPause();
        locationManager.removeUpdates(locationListener);
    }

    private LocationListener locationListener = new LocationListener() {

        @Override
        public void onLocationChanged(Location location) {
            showLocation(location);
        }

        @Override
        public void onProviderDisabled(String provider) {
            checkEnabled();
        }

        @Override
        public void onProviderEnabled(String provider) {
            checkEnabled();
            //showLocation(locationManager.getLastKnownLocation(provider));
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {
            if (provider.equals(LocationManager.GPS_PROVIDER)) {
                tvStatusGPS.setText("Status: " + String.valueOf(status));
            }
        }
    };

    private void showLocation(Location location) {
        if (location == null)
            return;
        if (location.getProvider().equals(LocationManager.GPS_PROVIDER)) {
            tvLocationGPS.setText(formatLocation(location));
        }
    }

    private String formatLocation(Location location) {
        if (location == null)
            return "";
        return String.format(
                "Coordinates: lat = %1$.7f, lon = %2$.7f, time = %3$tF %3$tT",
                location.getLatitude(), location.getLongitude(), new Date(location.getTime()));
//                location.getLatitude(), location.getLongitude(), location.);
    }

    private void checkEnabled() {
        tvEnabledGPS.setText("Enabled: " + locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER));
    }

    public void onClickLocationSettings(View view) {
        startActivity(new Intent(
                android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS));
    };

}