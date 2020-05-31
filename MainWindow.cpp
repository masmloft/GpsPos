#include "MainWindow.h"

#include "MapWidget/GoogleTileManager.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , _geoPositionInfoSource(NULL)
    , _geoReqCount(0)
  , _geoPassCount(0)
  , _isReq(false)
  , _reqTimeout(true)
{
    this->setObjectName("MainWindow");

    {
        _geoWidget = new GeoWidget(this);
        setCentralWidget(_geoWidget);
        _geoWidget->setTileManager(new GoogleTileManager(this));
    }

    startLocation();
}

MainWindow::~MainWindow()
{
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    _reqTimeout = true;
    geoRequestUpdate();
}

void MainWindow::startLocation()
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

void MainWindow::geoRequestUpdate()
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
}

void MainWindow::geoError(QGeoPositionInfoSource::Error positioningError)
{
//    ui->geoErrorWidget->setText(tr("Err: %1").arg(positioningError));

    _isReq = false;
    geoRequestUpdate();
}

void MainWindow::geoPositionUpdated(const QGeoPositionInfo& geoPositionInfo)
{
    _currentGeoPositionInfo = geoPositionInfo;
    _geoWidget->setLatLonDeg(_currentGeoPositionInfo.coordinate().latitude(), _currentGeoPositionInfo.coordinate().longitude());
    updateLatLonWidget();

    _isReq = false;
    geoRequestUpdate();
}

void MainWindow::geoUpdateTimeout()
{
    _geoPassCount++;
//    ui->geoStatusWidget->setText(tr("Timeout"));

    _isReq = false;
    geoRequestUpdate();
}

void MainWindow::updateLatLonWidget()
{
    if(_currentGeoPositionInfo.isValid() == false)
    {
//        ui->geoStatusWidget->setText(tr("Not Valid"));
//        ui->latWidget->setText(tr("%1").arg(tr("Not Valid")));
//        ui->lonWidget->setText(tr("%1").arg(tr("Not Valid")));
        return;
    }

//    ui->geoStatusWidget->setText(tr("OK"));
//    ui->latWidget->setText(tr("%1").arg(_currentGeoPositionInfo.coordinate().latitude(), 0, 'f'));
//    ui->lonWidget->setText(tr("%1").arg(_currentGeoPositionInfo.coordinate().longitude(), 0, 'f'));
}
