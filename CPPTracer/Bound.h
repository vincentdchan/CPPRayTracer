#pragma once
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

class Bound final
{
public:
	class iterator;

private:
	Vector2i _left_top;
	Vector2i _right_bottom;

public:
	Bound();
	Bound(const Vector2i& left_top, const Vector2i& right_bottom);
	Bound::iterator begin() const;
	Bound::iterator end() const;

	inline const Vector2i& get_left_top() const
	{
		return _left_top;
	}

	inline const Vector2i& get_right_bottom() const
	{
		return _right_bottom;
	}

	inline int get_pixels_size() const
	{
		return (_right_bottom(0) - _left_top(0) + 1) * 
			(_right_bottom(1) - _left_top(1) + 1);
	}

	inline int get_width() const
	{
		return _right_bottom(0) - _left_top(0);
	}

	inline int get_height() const
	{
		return _right_bottom(1) - _left_top(1);
	}

};

class Bound::iterator final
{
private:
	int x;
	int y;
	const Bound* _father;

	iterator(const Bound * fater);
	iterator(const Bound * fater, int x, int y);

public:
	iterator(const iterator&);
	bool operator <(const iterator&) const;
	bool operator >(const iterator&) const;
	bool operator !=(const iterator&) const;
	Vector2i operator *() const;
	Bound::iterator& operator ++();
	Bound::iterator& operator =(const iterator&);

	friend class Bound;

};
