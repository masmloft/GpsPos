#pragma once

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class GpsSource : public QObject
{
    Q_OBJECT
public:
    explicit GpsSource(QObject* parent);
    ~GpsSource();
public:
    void startLocation();
protected:
    void timerEvent(QTimerEvent* event) override;
private:
    enum State
    {
        STATE_NULL = 0,
        STATE_IS_REQ,
        STATE_DO_REQ,
    };
private:
    QGeoPositionInfoSource* _geoPositionInfoSource = nullptr;
    QGeoPositionInfo _currentGeoPositionInfo;
    int _geoReqCount = 0;
    int _geoPassCount = 0;
    bool _isReq = false;
    bool _reqTimeout = true;
private slots:
    void geoError(QGeoPositionInfoSource::Error positioningError);
    void geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo);
    void geoUpdateTimeout();
private:
    void updateData();
};
