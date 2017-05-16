#pragma once
#include <algorithm>
#include "Color.h"

class Tile
{
private:
	int width;
	int height;
	int len;
	int ptr;

	unsigned char* data;

public:
	Tile();
	Tile(int w, int h);
	Tile(const Tile&) = delete;
	Tile(Tile&&);

	Tile& operator=(const Tile&) = delete;
	Tile& operator=(Tile&&);

	inline const unsigned char*
		get_data() const
	{
		return data;
	}

	inline unsigned char& 
		operator[](int index) const
	{
		return data[index];
	}

	inline void push_pixel(char b, char g, char r, char alpha)
	{
		data[ptr++] = b;
		data[ptr++] = g;
		data[ptr++] = r;
		data[ptr++] = alpha;
	}

	inline void push_color(const Color& color)
	{
		data[ptr++] = std::min<int>(color(2) * 255, 255);
		data[ptr++] = std::min<int>(color(1) * 255, 255);
		data[ptr++] = std::min<int>(color(0) * 255, 255);
		data[ptr++] = 255;
	}

	inline int get_width() const { return width; }
	inline int get_height() const { return height; }
	inline int size() const { return len; }

	~Tile();

};
