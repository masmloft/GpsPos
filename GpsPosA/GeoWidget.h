#pragma once

#include <list>

#include "Classes/GpsSatInfo.h"
#include "MapWidget/MapWidget.h"

class GeoWidget : public MapWidget
{
public:
	GeoWidget(QWidget* parent);
	~GeoWidget();
public:
	void setGpsPoint(const QVariantMap& data);
	void setGpsSatInfo(const std::vector<GpsSatInfo>& info);
protected:
	void paintEvent(QPaintEvent *event) override;
private:
	struct GpsPoint
	{
		bool blocked = false;
		int64_t tick = 0;
		QByteArray cid;
		bool gpsValid = false;
		int64_t gpsTime = 0;
		LatLonPoint gpsPos;
		double alt;
		uint okCount = 0;
		uint errCount = 0;
		uint timeoutCount = 0;
	};
private:
	std::list<GpsPoint> _gpsPoints;
	std::vector<GpsSatInfo> _gpsSatInfos;
//	LatLonPoint _selfPos;
//    uint _okCount = 0;
//    uint _errCount = 0;
//    uint _timeoutCount = 0;
};
