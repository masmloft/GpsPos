#pragma once

#include <QPixmap>
#include <QList>
#include <QMap>

#include "Tile.h"


class TileImage
{
public:
	TileImage(const Tile& tile)
		: _tile(tile)
		, _image()
	{

	}

	const Tile& tile() const
	{
		return _tile;
	}

	const QPixmap& image() const
	{
		return _image;
	}

	QPixmap& image()
	{
		return _image;
	}

private:
	Tile _tile;
	QPixmap _image;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class TileImageList
{
public:
	~TileImageList()
	{
		for(Data::iterator i = _data.begin(); i != _data.end(); ++i)
			delete (*i);
		for(Data::iterator i = _oldData.begin(); i != _oldData.end(); ++i)
			delete (*i);
	}

	void debug()
	{
		return;
		qDebug() << "----";
		for(int i = 0; i < _data.size(); ++i)
		{
			TileImage* tileImage = _data.at(i);
			qDebug() << i+1 << ". " << tileImage->tile().z() << "-" << tileImage->tile().x() << "-" << tileImage->tile().y();
		}
	}

	TileImage* tileImage(const Tile& tile)
	{
		for(Data::iterator i = _data.begin(); i != _data.end(); ++i)
		{
			TileImage* tileImage = *i;
			if (tileImage->tile() == tile)
			{
				return tileImage;
			}
		}

		return NULL;
	}

	void beginReqTile()
	{
		_oldData += _data;
		_data.clear();
	}

	TileImage* reqTileImage(const Tile& tile)
	{
		TileImage* tileImage = NULL;

		//reverse
		for(Data::iterator i = _oldData.begin(); i != _oldData.end(); ++i)
		{
			if ((*i)->tile() == tile)
			{
				tileImage = (*i);
				_oldData.erase(i);
				break;
			}
		}

		if (tileImage == NULL)
		{
			tileImage = new TileImage(tile);
		}
		
		_data.append(tileImage);

		debug();

		return tileImage;
	}

	void endReqTile()
	{
		const int eraseCount = _oldData.size() - 64;
		if (eraseCount <= 0)
			return;

		for(int i = 0; i < eraseCount; ++i)
		{
			static int delCount = 0;
			delCount++;

			TileImage* tileImage = _oldData.at(i);
			delete tileImage;
		}

		_oldData.erase(_oldData.begin(), _oldData.begin() + eraseCount);
		
	}

	const Tile* firstEmptyTileImage() const
	{
		for(Data::const_iterator i = _data.constBegin(); i != _data.constEnd(); ++i)
		{
			TileImage* tileImage = *i;
			if (tileImage->image().isNull())
				return &(tileImage->tile());
		}		
		return NULL;
	}

private:
	typedef QList<TileImage*> Data;

private:
	Data _data;
	Data _oldData;
};
