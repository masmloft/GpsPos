#pragma once

#include <QList>


class Tile
{
public:
	typedef quint64 TileId;

public:
	Tile()
		: _data(0)
	{
	}

	Tile(int x, int y, int z)
		: _data(x, y, z)
	{
	}

	Tile(TileId id)
		: _data(id)
	{
	}

	Tile(const Tile& tile)
		: _data(tile._data.id)
	{
	}

	Tile& operator=(TileId id)
	{
		_data.id = id; 
		return *this;
	}

	Tile& operator=(const Tile &tile)
	{	
		_data.id = tile._data.id; 
		return *this;
	}

	bool operator==(const Tile& tile) const
	{
		return (tile._data.id == this->_data.id);
	}

	TileId id() const 
	{
		return _data.id;
	}
	
	void setId(TileId id)
	{
		_data.id = id;
	}

	int x() const 
	{
		return _data.x;
	}

	void setX(int x) 
	{
		_data.x = x;
	}

	int y() const 
	{
		return _data.y;
	}

	void setY(int y)
	{
		_data.y = y;
	}
	
	int z() const 
	{
		return _data.z;
	}

	void setZ(int z)
	{
		_data.z = z;
	}

private:
	union Data
	{
		quint64 id;

		struct
		{
			quint64 x : 28;
			quint64 y : 28;
			quint64 z : 8;
		};

		Data(quint64 aid)
			: id(aid)
		{
		}

		Data(int ax, int ay, int az)
			: x(ax)
			, y(ay)
			, z(az)
		{
		}
	};

private:
	Data _data;
};

inline bool operator<(const Tile& v1, const Tile& v2)
{
	return (v1.id() < v2.id());
}

typedef QList<Tile> TileList;
