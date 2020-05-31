#pragma once

#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "TileManager.h"


class GoogleTileManager : public TileManager
{
	Q_OBJECT
public:
	GoogleTileManager(QObject* parent);
	~GoogleTileManager();

protected:
	virtual bool reqTileFromNetwork(const Tile& tile);

private:
	QNetworkAccessManager* _networkAccessManager;
	QNetworkRequest _request;
	Tile _reqTile;
	bool _busy;

private slots:
	void replyFinished(QNetworkReply* reply);
};
