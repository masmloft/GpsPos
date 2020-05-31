#pragma once

#include <QWidget>
#include <QLabel>

#include "MapPoint.h"
#include "TileManager.h"


class MapWidget : public QWidget
{
	Q_OBJECT
public:
	MapWidget(QWidget* parent);
	~MapWidget();

	void setTileManager(TileManager* tileManager);

	const MapPoint& centerMapPoint() const
	{
		return _centerMapPoint; 
	}

	void setCenterMapPoint(const MapPoint& val)
	{
		_centerMapPoint = val;
		update();
	}

	QPoint latLonPointToPos(const LatLonPoint& latLonPoint)
	{
		QPoint ret(MapPoint::lonToX(_centerMapPoint.zoom(), latLonPoint.lon()), MapPoint::latToY(_centerMapPoint.zoom(), latLonPoint.lat()));
		ret.rx() = width() / 2 + (ret.x() - _centerMapPoint.x());
		ret.ry() = height() / 2 + (ret.y() - _centerMapPoint.y());
		return ret;
	}

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

//	QPoint calcPaintPoint(const Tile& tile);

private:
	TileManager* _tileManager;
	QLabel* _statusWidget;
	QLabel* _zoomWidget;
	MapPoint _centerMapPoint;
	MapPoint _mouseMapPoint;

private:
	void updateStatusWidget();
	void updateMPusePos(const QPoint& pos);

private slots:
	void tileImageChanged();
	void on_zoomInPushButton_clicked();
	void on_zoomOutPushButton_clicked();
};
