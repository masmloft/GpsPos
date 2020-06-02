#pragma once

#include "MapWidget/MapWidget.h"


class GeoWidget : public MapWidget
{
public:
	GeoWidget(QWidget* parent);
	~GeoWidget();

	void setLatLon(double lat, double lon);
	void setLatLonDeg(double lat, double lon);
    void setStat(uint okCount, uint errCount, uint timeoutCount);

protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	LatLonPoint _selfPos;
    uint _okCount = 0;
    uint _errCount = 0;
    uint _timeoutCount = 0;
};
