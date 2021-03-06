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

	QPainter painter(this);

	{
		QSize c = this->size() / 2;
		painter.drawLine(c.width() - 10, c.height(), c.width() + 10, c.height());
		painter.drawLine(c.width(), c.height() - 10, c.width(), c.height() + 10);
	}

	{
//			QString str = tr("o:%1 e:%2 t:%3").arg(gpsPoint.okCount).arg(gpsPoint.errCount).arg(gpsPoint.timeoutCount);
//            painter.drawText(2, 20, str);
	}

	{
		QString str = "Info: " + QString::number(_gpsSatInfos.size()) + '\n';

		for(const GpsSatInfo& item : _gpsSatInfos)
		{
			str += QString::number(item.id);
			str += " : ";
			str += QString::number(item.signalStrength);
			str += '\n';
		}
		painter.drawText(0, 20, width(), height() - 20,  Qt::AlignLeft | Qt::TextWordWrap, str);
	}


	for(const auto& gpsPoint : _gpsPoints)
    {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        static const int POINT_RADIUS = 12;
		QPoint pos = latLonPointToPos(gpsPoint.gpsPos);
        painter.drawEllipse(pos, POINT_RADIUS, POINT_RADIUS);

        //painter.drawText(3, 3, tr("%1 %2").arg(_selfPos.lat()).arg(_selfPos.lon()));
        {
			QString str = tr("%1 %2 %3 %4 %5");
			str = str.arg(QString(gpsPoint.cid));
			str = str.arg(QDateTime::fromMSecsSinceEpoch(gpsPoint.gpsTime).toString("hh:mm:ss"));
			str = str.arg(MATH_RADTODEG(gpsPoint.gpsPos.lat()), 0, 'f', 6);
			str = str.arg(MATH_RADTODEG(gpsPoint.gpsPos.lon()), 0, 'f', 6);
			str = str.arg(gpsPoint.alt, 0, 'f', 1);
			painter.drawText(pos.x() + 2 + POINT_RADIUS, pos.y(), str);
        }
    }
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
	gpsPoint.alt = data.value("alt", 0).toDouble();
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
//	repaint();
}

void GeoWidget::setGpsSatInfo(const std::vector<GpsSatInfo>& info)
{
	_gpsSatInfos = info;
//	repaint();
	update();
}
