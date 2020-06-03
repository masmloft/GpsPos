#pragma once

#include <list>

#include "MapWidget/MapWidget.h"

class GeoWidget : public MapWidget
{
public:
	GeoWidget(QWidget* parent);
	~GeoWidget();

//	void setLatLon(double lat, double lon);
//	void setLatLonDeg(double lat, double lon);
//    void setStat(uint okCount, uint errCount, uint timeoutCount);

	void setGpsPoint(const QVariantMap& data);
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	struct GpsPoint
	{
		bool blocked = false;
		int64_t tick = 0;
		QByteArray cid;
		bool gpsValid = false;
		int64_t gpsTime = 0;
		LatLonPoint gpsPos;
		uint okCount = 0;
		uint errCount = 0;
		uint timeoutCount = 0;
	};
private:
	std::list<GpsPoint> _gpsPoints;
//	LatLonPoint _selfPos;
//    uint _okCount = 0;
//    uint _errCount = 0;
//    uint _timeoutCount = 0;
};
