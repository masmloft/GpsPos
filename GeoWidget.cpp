#include "GeoWidget.h"

#include <QPainter>
#include <QMouseEvent>


GeoWidget::GeoWidget(QWidget* parent)
: MapWidget(parent)
{
	setCenterMapPoint(MapPoint(MATH_DEGTORAD(50.444154), MATH_DEGTORAD(30.410335), 12));
	_selfPos.setLatLonDeg(50.444154, 30.410335);
}

GeoWidget::~GeoWidget()
{

}

void GeoWidget::paintEvent(QPaintEvent *event)
{
	MapWidget::paintEvent(event);

	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::green);

	static const int POINT_RADIUS = 12;

	QPoint pos = latLonPointToPos(_selfPos);
/*
	pos.rx() -= POINT_RADIUS;
	pos.ry() -= POINT_RADIUS;
*/

	painter.drawEllipse(pos, POINT_RADIUS, POINT_RADIUS);

//	_selfPos.rlon() += 0.001;

}

void GeoWidget::setLatLon(double lat, double lon)
{
	_selfPos.setLatLon(lat, lon);
	update();
}

void GeoWidget::setLatLonDeg(double lat, double lon)
{
	setLatLon(MATH_DEGTORAD(lat), MATH_DEGTORAD(lon));
}
