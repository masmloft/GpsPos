#include "GoogleTileManager.h"

#include <QNetworkReply>
#include <QDebug>


GoogleTileManager::GoogleTileManager(QObject* parent)
: TileManager(parent)
, _networkAccessManager(NULL)
, _request()
, _reqTile()
, _busy(false)
{
	{
		_networkAccessManager = new QNetworkAccessManager(this);
		connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	}

	_request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:53.0) Gecko/20100101 Firefox/53.0");
}

GoogleTileManager::~GoogleTileManager()
{
	if (_networkAccessManager != NULL)
	{
		delete _networkAccessManager;
	}
}

bool GoogleTileManager::reqTileFromNetwork(const Tile& tile)
{
	if (_busy == true)
		return false;

//	qDebug() << "Req " << tile.z() << "-" << tile.x() << "-" << tile.y();

	_reqTile = tile;

	QString urlStr = QString("http://mt.google.com/vt/lyrs=m&x=%1&y=%2&z=%3").arg(tile.x()).arg(tile.y()).arg(tile.z());

	_request.setUrl(QUrl(urlStr));

	_networkAccessManager->get(_request);

	_busy = true;

	return true;
}

void GoogleTileManager::replyFinished(QNetworkReply* reply)
{
	_busy = false;

	if (reply->error() != QNetworkReply::NoError)
		return;

	QByteArray buf = reply->readAll();

	if (buf.isEmpty())
	{
		qDebug() << "Res Empty " << _reqTile.z() << "-" << _reqTile.x() << "-" << _reqTile.y();
		resTileFromNetwork(_reqTile, buf);
		return;
	}

//	qDebug() << "Res " << _reqTile.z() << "-" << _reqTile.x() << "-" << _reqTile.y();
	resTileFromNetwork(_reqTile, buf);
}

