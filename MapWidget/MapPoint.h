#pragma once

#include <QtGlobal>
#include <QPoint>


#ifndef MATH_PI
#define MATH_PI (3.14159265358979323846)
#endif

#ifndef MATH_2PI
#define MATH_2PI (2 *MATH_PI)
#endif

#ifndef MATH_PI2
#define MATH_PI2 (MATH_PI / 2)
#endif

#ifndef MATH_RADTODEG
#define MATH_RADTODEG(val) (val * 180 / MATH_PI)
#endif

#ifndef MATH_DEGTORAD
#define MATH_DEGTORAD(val) (val * MATH_PI / 180)
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
class LatLonPoint
{
public:
	LatLonPoint()
		: _lat(0.0)
		, _lon(0.0)
	{
	}

	LatLonPoint(double lat, double lon)
		: _lat(lat)
		, _lon(lon)
	{
	}

	bool isNull() const 
	{ 
		return ((_lat == 0.0) && (_lon == 0.0)); 
	}

	double lat() const 
	{ 
		return _lat; 
	}

	double& rlat()
	{ 
		return _lat; 
	}

	void setLat(double value) 
	{ 
		_lat = value;
	}

	double lon() const 
	{ 
		return _lon; 
	}

	double& rlon()
	{ 
		return _lon; 
	}

	void setLon(double value) 
	{
		_lon = value;
	}

	void setLatDeg(double value) 
	{ 
		setLat(MATH_DEGTORAD(value));
	}

	void setLonDeg(double value) 
	{ 
		setLon(MATH_DEGTORAD(value));
	}

	void setLatLon(double lat, double lon)
	{ 
		_lat = lat;
		_lon = lon;
	}

	void setLatLonDeg(double lat, double lon) 
	{ 
		setLatLon(MATH_DEGTORAD(lat), MATH_DEGTORAD(lon));
	}

private:
	double _lat;
	double _lon;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class MapPoint
{
public:
	static const qint32 PIX_PER_ZOOM[21];

public:
	static double yToLat(qint32 zoom, qint32 value);
	static double xToLon(qint32 zoom, qint32 value);
	static qint32 latToY(qint32 zoom, double value);
	static qint32 lonToX(qint32 zoom, double value);

public:
	MapPoint()
		: _lat(0.0)
		, _lon(0.0)
		, _zoom(0)
	{
	}

	MapPoint(int zoom)
		: _lat(0.0)
		, _lon(0.0)
		, _zoom(zoom)

	{
		verifyZoom();
	}

	MapPoint(double lat, double lon, qint32 zoom)
		: _lat(lat)
		, _lon(lon)
		, _zoom(zoom)
	{
		verifyLat();
		verifyLon();
		verifyZoom();
	}

	MapPoint(qint32 x, qint32 y, qint32 zoom)
		: _zoom(zoom)
	{
		_lat = yToLat(_zoom, y);
		verifyLat();
		_lon = xToLon(_zoom, x);
		verifyLon();
		verifyZoom();
	}

	MapPoint(const LatLonPoint& point, int zoom)
		: _lat(point.lat())
		, _lon(point.lon())
		, _zoom(zoom)
	{
		verifyLat();
		verifyLon();
		verifyZoom();
	}

	bool isNull() const 
	{ 
		return ((_lat == 0.0) && (_lon == 0.0)); 
	}

	double lat() const 
	{ 
		return _lat; 
	}
	
	double lon() const 
	{ 
		return _lon; 
	}

	qint32 zoom() const 
	{ 
		return _zoom; 
	}

	void setLat(double value) 
	{ 
		_lat = value;
		verifyLat(); 
	}

	void setLon(double value) 
	{
		_lon = value;
		verifyLon(); 
	}

	void setLatDeg(double value) 
	{ 
		setLat(MATH_DEGTORAD(value));
	}

	void setLonDeg(double value) 
	{ 
		setLon(MATH_DEGTORAD(value));
	}

	void setLatLon(double lat, double lon) 
	{ 
		setLat(lat);
		setLon(lon);
	}

	void setLatLonDeg(double lat, double lon) 
	{ 
		setLatLon(MATH_DEGTORAD(lat), MATH_DEGTORAD(lon));
	}

	void setZoom(qint32 value) 
	{ 
		_zoom = value; 
		verifyZoom();
	}
	
	void addZoom(qint32 value) 
	{ 
		_zoom += value; 
		verifyZoom();
	}

	qint32 x() const
	{
		return lonToX(_zoom, _lon);
	}
	
	qint32 y() const 
	{
		return latToY(_zoom, _lat);
	}

	QPoint point() const
	{
		return QPoint(lonToX(_zoom, _lon), latToY(_zoom, _lat));
	}

	void setX(qint32 value)
	{
		_lon = xToLon(_zoom, value);
		verifyLat();
	}
	
	void setY(qint32 value)
	{
		_lat = yToLat(_zoom, value);
		verifyLon();
	}
	
	void setPoint(const QPoint &value);
	void addPoint(const QPoint &value);
	void addZoom(qint32 &value);

	MapPoint& operator=(const MapPoint &value)
	{
		this->_lat = value._lat; 
		this->_lon = value._lon; 
		this->_zoom = value._zoom; 
		return *this;
	}

	static double radToMeter(double lat1, double lon1, double lat2, double lon2);

protected:
	void verifyLat();
	void verifyLon();
	void verifyZoom();

private:
	double _lat;
	double _lon;
	qint32 _zoom;
};
