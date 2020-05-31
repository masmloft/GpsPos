#include "MapPoint.h"

#include <Math.h>

#define MAX_LAT (MATH_DEGTORAD(89))
#define MIN_LAT (-MATH_DEGTORAD(89))
#define MAX_LON (MATH_PI)
#define MIN_LON (-MATH_PI)

#define MAX_ZOOM (20)
#define MIN_ZOOM (0)

const qint32 MapPoint::PIX_PER_ZOOM[21] = {
	256, 
	512, 
	1024, 
	2048, 
	4096, 
	8192, 
	16384, 
	32768, 
	65536, 
	131072, 
	262144, 
	524288, 
	1048576,
	2097152,
	4194304,
	8388608,
	16777216,
	33554432,
	67108864,
	134217728,
	268435456
};


double MapPoint::xToLon(qint32 zoom, qint32 value)
{
	double ret = MATH_2PI * (value - (PIX_PER_ZOOM[zoom] / 2)) / PIX_PER_ZOOM[zoom]; 
	return ret;
}

qint32 MapPoint::lonToX(qint32 zoom, double value)
{
	double ret = PIX_PER_ZOOM[zoom] / 2 + value * PIX_PER_ZOOM[zoom] / MATH_2PI;
	return floor(ret + 0.5);
}

double MapPoint::yToLat(qint32 zoom, qint32 value)
{
	double ret = 2 * atan(exp(((PIX_PER_ZOOM[zoom] / 2) - value) * 2 * MATH_PI / PIX_PER_ZOOM[zoom])) - MATH_PI / 2;
	return ret;
}

qint32 MapPoint::latToY(qint32 zoom, double value)
{
	double ret = (PIX_PER_ZOOM[zoom] / 2) - 0.5 * log((1 + sin(value)) / (1 - sin(value))) * PIX_PER_ZOOM[zoom] / MATH_2PI;
	return floor(ret + 0.5);
}

void MapPoint::verifyLat()
{
	if (_lat > MAX_LAT)
		_lat = MAX_LAT;
	if (_lat < MIN_LAT)
		_lat = MIN_LAT;
}

void MapPoint::verifyLon()
{
	if (_lon > MAX_LON)
		_lon = MAX_LON;
	if (_lon < MIN_LON)
		_lon = MIN_LON;
}

void MapPoint::verifyZoom()
{
	if (_zoom > MAX_ZOOM)
		_zoom = MAX_ZOOM;
	if (_zoom < MIN_ZOOM)
		_zoom = MIN_ZOOM;
}

double MapPoint::radToMeter(double lat1, double lon1, double lat2, double lon2)
{
	return 6378135 * acos(sin(lon1) * sin(lon2) + cos(lon1) * cos(lon2) * cos(lat1 - lat2));
}

void MapPoint::setPoint(const QPoint &value)
{
	_lat = yToLat(_zoom, value.y());
	verifyLat();
	_lon = xToLon(_zoom, value.x());
	verifyLon();
};

void MapPoint::addPoint(const QPoint &value)
{
	setX(x() + value.x());
	setY(y() + value.y());
	verifyLat();
	verifyLon();
};
