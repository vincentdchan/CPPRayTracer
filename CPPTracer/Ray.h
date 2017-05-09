#pragma once
#include <Eigen/Dense>

using namespace Eigen;

class Ray 
{
private:

	Vector3f _origin;
	Vector3f _direction;

public:
	
	Ray(const Vector3f& origin, const Vector3f& direction):
		_origin(origin), _direction(direction)
	{}

	Ray(const Ray& that):
		_origin(that._origin), _direction(that._direction)
	{ }

	Ray& operator=(const Ray& that)
	{
		_origin = that._origin;
		_direction = that._direction;
	}

	Vector3f get_point(float t) const
	{
		return _origin + _direction * t;
	}

	const Vector3f& get_origin() const
	{
		return _origin;
	}

	const Vector3f& get_direction() const
	{
		return _direction;
	}

};
