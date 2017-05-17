#include <cstring>
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

int Tile::get_offset(int x, int y) const
{
	return (y * width + x) * 4;
}

int Tile::get_offset(const Vector2i& point) const
{
	return get_offset(point(0), point(1));
}

void Tile::merge(const Bound& bound, const Tile& tile)
{
	for (Vector2i point : bound)
	{
		Vector2i relative = point - bound.get_left_top();
		unsigned char * this_ptr = data + get_offset(point);
		unsigned char * target_ptr = tile.data + tile.get_offset(relative);
		memcpy(this_ptr, target_ptr, sizeof(char) * 4);
	}
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
