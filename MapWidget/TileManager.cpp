#include "TileManager.h"

#include <QTimerEvent>
#include <QPainter>


TileManager::TileManager(QObject* parent)
: QObject(parent)
, _tileImageList()
, _timerId(0)
{
	
}

TileManager::~TileManager()
{

}

void TileManager::timerEvent(QTimerEvent* event)
{
	return;

	const Tile* tile = _tileImageList.firstEmptyTileImage();
	if (tile == NULL)
	{
		_timerId = 0;
		killTimer(event->timerId());
		return;
	}

	TileImage* tileImage = _tileImageList.tileImage(*tile);

	tileImage->image() = QPixmap(QSize(256, 256));
	QPainter painter(&(tileImage->image()));
	painter.drawText(20, 20, tr("%1-%2-%3").arg(tile->z()).arg(tile->x()).arg(tile->y()));

	//	qDebug() << "Load " << tileImage->tile().z() << "-" << tileImage->tile().x() << "-" << tileImage->tile().y();

	emit(tileImageChanged());
}

void TileManager::beginReqTile()
{
	_tileImageList.beginReqTile();
}

const TileImage* TileManager::reqTile(const Tile& tile)
{
	TileImage* tileImage = _tileImageList.reqTileImage(tile);
	if (tileImage == NULL)
		return NULL;

	if (tileImage->image().isNull())
		return NULL;

	return tileImage;
}

void TileManager::endReqTile()
{
	_tileImageList.endReqTile();

	const Tile* tile = _tileImageList.firstEmptyTileImage();
	if (tile != NULL)
	{
//		if(_timerId == 0)
//			_timerId = startTimer(50);
		reqTileFromNetwork(*tile);
	}
}

void TileManager::resTileFromNetwork(const Tile& tile, const QByteArray& buf)
{
	TileImage* tileImage = _tileImageList.tileImage(tile);
	if (tileImage == NULL)
		return;

	if (buf.isEmpty())
	{
		tileImage->image() = QPixmap(QSize(256, 256));
		QPainter painter(&(tileImage->image()));
		painter.drawText(20, 20, tr("Empty %1-%2-%3").arg(tile.z()).arg(tile.x()).arg(tile.y()));
		emit(tileImageChanged());
		return;
	}

	tileImage->image().loadFromData(buf);

	emit(tileImageChanged());

	{
		const Tile* tile = _tileImageList.firstEmptyTileImage();
		if (tile != NULL)
			reqTileFromNetwork(*tile);
	}

}
