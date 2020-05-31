#include "MainWindow.h"

#include "MapWidget/GoogleTileManager.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setObjectName("MainWindow");

    {
        _geoWidget = new GeoWidget(this);
        setCentralWidget(_geoWidget);
        _geoWidget->setTileManager(new GoogleTileManager(this));
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::gpsChanged(const QVariantMap& data)
{
    bool valid = data.value("valid", false).toBool();
    if(valid)
    {
        double lat = data.value("lat", 0).toDouble();
        double lon = data.value("lon", 0).toDouble();
        _geoWidget->setLatLonDeg(lat, lon);
    }

    {
        uint okCount = data.value("okCount", 0).toUInt();
        uint errCount = data.value("errCount", 0).toUInt();
        uint timeoutCount = data.value("timeoutCount", 0).toUInt();

        _geoWidget->setStat(okCount, errCount, timeoutCount);
    }
}
