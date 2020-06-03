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
	_geoWidget->setGpsPoint(data);
}
