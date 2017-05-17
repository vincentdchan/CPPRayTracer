#pragma once
#include <algorithm>
#include "Bound.h"
#include "Color.h"

class Tile
{
private:
	int width;
	int height;
	int len;
	int ptr;

	unsigned char* data;

	int get_offset(int x, int y) const;
	int get_offset(const Vector2i& point) const;

public:
	Tile();
	Tile(int w, int h);
	Tile(const Tile&) = delete;
	Tile(Tile&&);

	Tile& operator=(const Tile&) = delete;
	Tile& operator=(Tile&&);

	void merge(const Bound&, const Tile&);

	inline const unsigned char*
		get_data() const { return data; }

	inline Color get_color(const Vector2i& point) const;
	inline Color get_color(int x, int y) const;
	inline unsigned char&
		operator[](int index) const;
	inline void push_pixel(char b, char g, char r, char alpha);
	inline void push_color(const Color& color);
	inline int get_width() const { return width; }
	inline int get_height() const { return height; }
	inline int size() const { return len; }

	~Tile();

};

Color Tile::get_color(const Vector2i& point) const
{
	return get_color(point(0), point(1));
}

Color Tile::get_color(int x, int y) const
{
	int p = (y * width + x) * 4;
	float b = static_cast<float>(data[p]) / 255;
	float g = static_cast<float>(data[p + 1]) / 255;
	float r = static_cast<float>(data[p + 2]) / 255;
	return Color(r, g, b);
}

inline unsigned char& 
	Tile::operator[](int index) const
{
	return data[index];
}

void Tile::push_pixel(char b, char g, char r, char alpha)
{
	data[ptr++] = b;
	data[ptr++] = g;
	data[ptr++] = r;
	data[ptr++] = alpha;
}

void Tile::push_color(const Color& color)
{
	data[ptr++] = static_cast<char>(std::min<int>(color(2) * 255, 255));
	data[ptr++] = static_cast<char>(std::min<int>(color(1) * 255, 255));
	data[ptr++] = static_cast<char>(std::min<int>(color(0) * 255, 255));
	data[ptr++] = static_cast<char>(255);
}
