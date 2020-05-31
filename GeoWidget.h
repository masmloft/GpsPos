#pragma once

#include "MapWidget/MapWidget.h"


class GeoWidget : public MapWidget
{
public:
	GeoWidget(QWidget* parent);
	~GeoWidget();

	void setLatLon(double lat, double lon);
	void setLatLonDeg(double lat, double lon);

protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	LatLonPoint _selfPos;
};
