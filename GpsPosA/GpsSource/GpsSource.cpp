#include "GpsSource.h"

#include <QTimerEvent>
#include <QDebug>

GpsSource::GpsSource(QObject* parent)
    : QObject(parent)
{
}

GpsSource::~GpsSource()
{
}

void GpsSource::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);

    if(event->timerId() == _reqTimerId)
    {
        killTimer(_reqTimerId);
        _reqTimerId = 0;
        requestUpdate();
        updateData();
    }
}

void GpsSource::startLocation()
{
    if (_geoPositionInfoSource == nullptr)
    {
//		QVariantMap parameters;
//		parameters["serial_port"] = QString("COM22");
//		_geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(parameters, this);
		_geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);

        if(_geoPositionInfoSource != nullptr)
        {
            _geoPositionInfoSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

            connect(_geoPositionInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT (geoError(QGeoPositionInfoSource::Error)));
            connect(_geoPositionInfoSource, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT (geoPositionUpdated(const QGeoPositionInfo&)));
            connect(_geoPositionInfoSource, SIGNAL(updateTimeout()), this, SLOT (geoUpdateTimeout()));

            //qDebug() << _geoPositionInfoSource->minimumUpdateInterval();
            int interval = qMax(_geoPositionInfoSource->minimumUpdateInterval(), 1000);
            _geoPositionInfoSource->setUpdateInterval(interval);
            _geoPositionInfoSource->startUpdates();
        }
    }
}

void GpsSource::updateData()
{
    {
        QVariantMap data;
		data["cid"] = QByteArray("MY");
		data["valid"] = _currentGeoPositionInfo.isValid();
		if(_currentGeoPositionInfo.timestamp().isValid())
			data["time"] = _currentGeoPositionInfo.timestamp().toMSecsSinceEpoch();
		if(_currentGeoPositionInfo.coordinate().isValid())
		{
			data["lat"] = _currentGeoPositionInfo.coordinate().latitude();
			data["lon"] = _currentGeoPositionInfo.coordinate().longitude();
			data["alt"] = _currentGeoPositionInfo.coordinate().altitude();
		}
		data["okCount"] = _geoOkCount;
        data["errCount"] = _geoErrCount;
        data["timeoutCount"] = _geoTimeoutCount;

        emit changed(data);
    }
}

void GpsSource::geoError(QGeoPositionInfoSource::Error positioningError)
{
    Q_UNUSED(positioningError);
    _geoErrCount++;
    _currentGeoPositionInfo.coordinate() = {};
	//qDebug() << __FUNCTION__ << _geoErrCount << " - "<< positioningError;
    if(_reqTimerId > 0)
        killTimer(_reqTimerId);
    _reqTimerId = startTimer(1000);

    updateData();
}

void GpsSource::geoUpdateTimeout()
{
    _geoTimeoutCount++;
    _currentGeoPositionInfo.coordinate() = {};

	//qDebug() << __FUNCTION__ << _geoTimeoutCount;

    if(_reqTimerId > 0)
        killTimer(_reqTimerId);
    _reqTimerId = startTimer(1000);

    updateData();
}

void GpsSource::geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo)
{
    _geoOkCount++;
	//qDebug() << __FUNCTION__ << _geoOkCount;
    _currentGeoPositionInfo = geoPositionInfo;
    updateData();
    requestUpdate();
}

void GpsSource::requestUpdate()
{
    if(_geoPositionInfoSource != nullptr)
    {
//        int timeout = qMax(_geoPositionInfoSource->minimumUpdateInterval(), 1000);
//        _geoPositionInfoSource->requestUpdate(timeout);
    }
}
