package com.example.gps3;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.GpsSatellite;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import java.util.ArrayList;
import java.util.Date;

public class Gps {
    final String LOG_TAG = "Gps";

    class Data {
        boolean enabledGPS = false;
        //int satellites = -1;
        double lat = 0;
        double lon = 0;
        double alt = 0;
        long date = 0;

        ArrayList<GpsSatellite> sats = new ArrayList<>();
    }

    interface Callback{
        void emit();
    }
////////////////////////////////////////////////////////////////////////////
    Callback callback;
////////////////////////////////////////////////////////////////////////////
    private Context _context;
    private LocationManager _locationManager;

    private Data _data = new Data();

    private LocationListener _locationListener = new LocationListener() {

        @Override
        public void onLocationChanged(Location location) {
            setLocation(location);
        }

        @Override
        public void onProviderDisabled(String provider) {
//            checkEnabled();
        }

        @Override
        public void onProviderEnabled(String provider) {
//            checkEnabled();
            //setLocation(_locationManager.getLastKnownLocation(provider));
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {
//            if (provider.equals(LocationManager.GPS_PROVIDER)) {
//                tvStatusGPS.setText("Status: " + String.valueOf(status));
//            }
        }
    };

    GpsStatus.Listener _gpsStatusListener = new GpsStatus.Listener() {
        @Override
        public void onGpsStatusChanged(int event) {
            if (event == GpsStatus.GPS_EVENT_SATELLITE_STATUS || event == GpsStatus.GPS_EVENT_FIRST_FIX) {
                GpsStatus status = _locationManager.getGpsStatus(null);
                Iterable<GpsSatellite> sats = status.getSatellites();
                // Check number of satellites in list to determine fix state

                _data.sats.clear();
                for (GpsSatellite sat : sats) {
                    _data.sats.add(sat);
                    //_data.satellites++;
                }

                Log.d(LOG_TAG, String.format("sats: %d", _data.sats.size()));

                if(callback != null)
                    callback.emit();
            }
        }
    };

    //////////////////////////////////////////////////////////
    public Gps(Context context) {
        this._context = context;
    }

    public void start()
    {
        stop();

        if (ActivityCompat.checkSelfPermission(_context, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED
                && ActivityCompat.checkSelfPermission(_context, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }

        if(_locationManager == null)
            _locationManager = (LocationManager) _context.getSystemService(Context.LOCATION_SERVICE);

        _locationManager.addGpsStatusListener(_gpsStatusListener);

        _locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000 * 1, 0, _locationListener);

        _data.enabledGPS = _locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
    }

    public void stop()
    {
        if(_locationManager != null)
        {
            //_locationManager.removeUpdates(_gpsStatusListener);
            _locationManager.removeUpdates(_locationListener);
        }
    }

    public Data data() {
        return _data;
    }

    private void setLocation(Location location) {
        if (location == null)
            return;
        if (location.getProvider().equals(LocationManager.GPS_PROVIDER)) {
            Log.d(LOG_TAG, "setLocation");
            _data.lat = location.getLatitude();
            _data.lon = location.getLongitude();
            _data.alt = location.getAltitude();
            _data.date = location.getTime();
            if(callback != null)
                callback.emit();
        }
    }
}
