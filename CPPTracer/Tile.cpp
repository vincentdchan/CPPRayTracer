#include "Tile.h"

Tile::Tile():
	width(0), height(0), ptr(0),
	len(0), data(nullptr)
{}

Tile::Tile(int w, int h):
	width(w), height(h), ptr(0), data(nullptr)
{
	len = width * height * 4;
	data = new unsigned char[len];
}

Tile::Tile(Tile&& that):
	width(that.width), height(that.height),
	len(that.len), ptr(that.ptr), data(that.data)
{
	that.data = nullptr;
}

Tile& Tile::operator=(Tile&& that)
{
	width = that.width;
	height = that.height;
	len = that.len;
	ptr = that.ptr;
	data = that.data;
	that.data = nullptr;
	return *this;
}

Tile::~Tile()
{
	if (data != nullptr)
		delete[] data;
}
