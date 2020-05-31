#pragma once

#include <QObject>
#include <QVariant>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class GpsSource : public QObject
{
    Q_OBJECT
public:
    struct Data
    {
        double lat;
        double lon;
    };
public:
    explicit GpsSource(QObject* parent);
    ~GpsSource();
public:
    void startLocation();
signals:
    //void changed(double lat, double lon);
    void changed(const QVariantMap& data);
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
    int _reqTimerId = 0;

//    int _timeoutTimerId = 0;
//    int _timeoutTime = 1000;
    QGeoPositionInfoSource* _geoPositionInfoSource = nullptr;
    QGeoPositionInfo _currentGeoPositionInfo;
    uint _geoOkCount = 0;
    uint _geoErrCount = 0;
    uint _geoTimeoutCount = 0;
private slots:
    void geoError(QGeoPositionInfoSource::Error positioningError);
    void geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo);
    void geoUpdateTimeout();
private:
    void requestUpdate();
    void updateData();
};
