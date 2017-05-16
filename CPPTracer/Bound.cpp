#include "Bound.h"

Bound::Bound()
{}

Bound::Bound(const Vector2i& left_top, const Vector2i& right_bottom):
	_left_top(left_top), _right_bottom(right_bottom)
{}

Bound::iterator
Bound::begin() const
{
	return iterator(this);
}

Bound::iterator
Bound::end() const
{
	return iterator(this, _left_top(0), _right_bottom(1) + 1);
}

Bound::iterator::iterator(const Bound * father):
	_father(father)
{
	x = father->_left_top(0);
	y = father->_left_top(1);
}

Bound::iterator::iterator(const Bound * father, int X, int Y):
	_father(father), x(X), y(Y)
{}

Bound::iterator::iterator(const iterator& that):
	x(that.x), y(that.y), _father(that._father)
{}


bool 
Bound::iterator::operator<(const iterator& that) const
{
	if (y < that.y) return true;
	else if (y > that.y) return false;
	else return x < that.x;
}

bool
Bound::iterator::operator>(const iterator& that) const
{
	if (y > that.y) return true;
	else if (y < that.y) return false;
	return x > that.x;
}

bool
Bound::iterator::operator!=(const iterator& that) const
{
	return y != that.y || x != that.x;
}

Vector2i
Bound::iterator::operator*() const
{
	return Vector2i(x, y);
}

Bound::iterator&
Bound::iterator::operator++()
{
	if (x <= _father->_right_bottom(0))
		x++;
	else
	{
		x = _father->_left_top(0);
		++y;
	}
	return *this;
}

Bound::iterator&
Bound::iterator::operator=(const iterator& that)
{
	x = that.x;
	y = that.y;
	_father = that._father;
	return *this;
}
