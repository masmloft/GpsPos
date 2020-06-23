#pragma once

#include <QMainWindow>

#include "GeoWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent);
    ~MainWindow();
public:
	void setGpsSatInfo(const std::vector<GpsSatInfo>& info) { _geoWidget->setGpsSatInfo(info); }
private:
    GeoWidget* _geoWidget;
public slots:
    void gpsChanged(const QVariantMap& data);
};
