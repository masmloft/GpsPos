#include "GpsSource.h"

GpsSource::GpsSource(QObject* parent)
    : QObject(parent)
{
    startLocation();
}

GpsSource::~GpsSource()
{
}

void GpsSource::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    _reqTimeout = true;
    updateData();
}

void GpsSource::startLocation()
{
    if (_geoPositionInfoSource == NULL)
    {
        _geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
        if(_geoPositionInfoSource == NULL)
            return;

        _geoPositionInfoSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);

        connect(_geoPositionInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT (geoError(QGeoPositionInfoSource::Error)));
        connect(_geoPositionInfoSource, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT (geoPositionUpdated(const QGeoPositionInfo&)));
        connect(_geoPositionInfoSource, SIGNAL(updateTimeout()), this, SLOT (geoUpdateTimeout()));

//        _geoPositionInfoSource->setUpdateInterval(1000);
        _geoPositionInfoSource->startUpdates();
        startTimer(1000);
    }
}

void GpsSource::updateData()
{
    if(_reqTimeout == false)
    {
        return;
    }

    if(_isReq == true)
    {
        return;
    }

    _reqTimeout = false;
    _isReq = true;

    if(_geoPositionInfoSource == NULL)
        return;

    int timeout = qMax(_geoPositionInfoSource->minimumUpdateInterval(), 1000);
    _geoPositionInfoSource->requestUpdate(timeout);

//    ui->geoReqCountWidget->setText(QString::number(++_geoReqCount) + "/" + QString::number(_geoPassCount));
//    ui->geoStatusWidget->setText(tr("Req"));

    emit changed(_currentGeoPositionInfo.coordinate().latitude(), _currentGeoPositionInfo.coordinate().longitude());
}

void GpsSource::geoError(QGeoPositionInfoSource::Error positioningError)
{
    Q_UNUSED(positioningError);
//    ui->geoErrorWidget->setText(tr("Err: %1").arg(positioningError));

    _isReq = false;
    updateData();
}

void GpsSource::geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo)
{
    _currentGeoPositionInfo = geoPositionInfo;
    //_geoWidget->setLatLonDeg(_currentGeoPositionInfo.coordinate().latitude(), _currentGeoPositionInfo.coordinate().longitude());

    _isReq = false;
    updateData();
}

void GpsSource::geoUpdateTimeout()
{
    _geoPassCount++;
//    ui->geoStatusWidget->setText(tr("Timeout"));

    _isReq = false;
    updateData();
}
