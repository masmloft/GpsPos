#pragma once

#include <QMainWindow>

#include "GeoWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent);
    ~MainWindow();
private:
    GeoWidget* _geoWidget;

public slots:
    void gpsChanged(const QVariantMap& data);
private:
};
