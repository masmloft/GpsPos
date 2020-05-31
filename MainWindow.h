#pragma once

#include <QMainWindow>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

#include "GeoWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent);
    ~MainWindow();

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
    QGeoPositionInfoSource* _geoPositionInfoSource;
    QGeoPositionInfo _currentGeoPositionInfo;
    int _geoReqCount;
    int _geoPassCount;
    bool _isReq;
    bool _reqTimeout;

    GeoWidget* _geoWidget;

private slots:
    void geoError(QGeoPositionInfoSource::Error positioningError);
    void geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo);
    void geoUpdateTimeout();
private:
    void startLocation();
    void geoRequestUpdate();
    void updateLatLonWidget();
};
