#include "GeoWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>


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

	for(const auto& gpsPoint : _gpsPoints)
    {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        static const int POINT_RADIUS = 12;
		QPoint pos = latLonPointToPos(gpsPoint.gpsPos);
        painter.drawEllipse(pos, POINT_RADIUS, POINT_RADIUS);

        {
            QSize c = this->size() / 2;
            painter.drawLine(c.width() - 5, c.height(), c.width() + 5, c.height());
            painter.drawLine(c.width(), c.height() - 5, c.width(), c.height() + 5);
        }

        //painter.drawText(3, 3, tr("%1 %2").arg(_selfPos.lat()).arg(_selfPos.lon()));
        {
			QString str = tr("%1: %2 %3 %4");
			str = str.arg(QString(gpsPoint.cid));
			str = str.arg(QDateTime::fromMSecsSinceEpoch(gpsPoint.gpsTime).toString(Qt::DateFormat::ISODate));
			str = str.arg(MATH_RADTODEG(gpsPoint.gpsPos.lat()), 0, 'f', 6);
			str = str.arg(MATH_RADTODEG(gpsPoint.gpsPos.lon()), 0, 'f', 6);
            painter.drawText(pos.x() + 2 + POINT_RADIUS, pos.y(), str);
        }
        {
			QString str = tr("o:%1 e:%2 t:%3").arg(gpsPoint.okCount).arg(gpsPoint.errCount).arg(gpsPoint.timeoutCount);
            painter.drawText(2, 20, str);
        }
    }

//	_selfPos.rlon() += 0.001;

}

void GeoWidget::setGpsPoint(const QVariantMap& data)
{
	GpsPoint gpsPoint;
	gpsPoint.blocked = false;
	gpsPoint.cid = data.value("cid", QByteArray()).toByteArray();
	gpsPoint.gpsValid = data.value("valid", false).toBool();
	gpsPoint.gpsTime = data.value("time", false).toLongLong();
	gpsPoint.gpsPos.rlat() = MATH_DEGTORAD(data.value("lat", 0).toDouble());
	gpsPoint.gpsPos.rlon() = MATH_DEGTORAD(data.value("lon", 0).toDouble());
	gpsPoint.okCount = data.value("okCount", 0).toUInt();
	gpsPoint.errCount = data.value("errCount", 0).toUInt();
	gpsPoint.timeoutCount = data.value("timeoutCount", 0).toUInt();

	auto findlm = [cid = gpsPoint.cid](auto item)
	{
		return (item.blocked == false) && (item.cid == cid);
	};

	auto it = std::find_if(_gpsPoints.begin(), _gpsPoints.end(), findlm);
	if(it != _gpsPoints.end())
	{
		*it = gpsPoint;
	}
	else
	{
		_gpsPoints.push_back(gpsPoint);
	}

	update();
}
