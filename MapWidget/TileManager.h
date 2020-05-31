#pragma once

#include <QObject>
#include <QDebug>

#include "MapPoint.h"
#include "Tile.h"
#include "TileImage.h"


class TileManager : public QObject
{
	Q_OBJECT
public:
	TileManager(QObject* parent);
	virtual ~TileManager();

	void beginReqTile();
	const TileImage* reqTile(const Tile& tile);
	void endReqTile();

signals:
	void tileImageChanged();

protected:
	virtual void timerEvent(QTimerEvent* event);
	
	virtual bool reqTileFromNetwork(const Tile& tile) = 0;
	void resTileFromNetwork(const Tile& tile, const QByteArray& buf);

private:
	TileImageList _tileImageList;
	int _timerId;
};
