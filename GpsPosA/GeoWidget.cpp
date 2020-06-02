#include "GeoWidget.h"

#include <QPainter>
#include <QMouseEvent>


GeoWidget::GeoWidget(QWidget* parent)
: MapWidget(parent)
{
	setCenterMapPoint(MapPoint(MATH_DEGTORAD(50.444154), MATH_DEGTORAD(30.410335), 12));
//	_selfPos.setLatLonDeg(50.444154, 30.410335);
}

GeoWidget::~GeoWidget()
{

}

void GeoWidget::paintEvent(QPaintEvent *event)
{
	MapWidget::paintEvent(event);

    {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        static const int POINT_RADIUS = 12;
        QPoint pos = latLonPointToPos(_selfPos);
        painter.drawEllipse(pos, POINT_RADIUS, POINT_RADIUS);

        {
            QSize c = this->size() / 2;
            painter.drawLine(c.width() - 5, c.height(), c.width() + 5, c.height());
            painter.drawLine(c.width(), c.height() - 5, c.width(), c.height() + 5);
        }

        //painter.drawText(3, 3, tr("%1 %2").arg(_selfPos.lat()).arg(_selfPos.lon()));
        {
            QString str = tr("%1 %2").arg(MATH_RADTODEG(_selfPos.lat()), 0, 'f', 6).arg(MATH_RADTODEG(_selfPos.lon()), 0, 'f', 6);
            painter.drawText(pos.x() + 2 + POINT_RADIUS, pos.y(), str);
        }
        {
            QString str = tr("o:%1 %2 %3").arg(_okCount).arg(_errCount).arg(_timeoutCount);
            painter.drawText(2, 20, str);
        }
    }

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

void GeoWidget::setStat(uint okCount, uint errCount, uint timeoutCount)
{
    _okCount = okCount;
    _errCount = errCount;
    _timeoutCount = timeoutCount;
    update();
}
