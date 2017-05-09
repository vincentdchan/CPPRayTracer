#pragma once
#include <Eigen/Dense>
#include "Ray.h"

#define PI 3.14159f

using namespace Eigen;

class PerspectiveCamera
{
private:
	Vector3f _eye;
	Vector3f _front;
	Vector3f _right;
	Vector3f _up;
	float _fov;
	float _fovScale;

public:

	PerspectiveCamera(const Vector3f& eye,
		const Vector3f& front,
		const Vector3f& up,
		float fov):
		_eye(eye), _front(front), _up(up), _fov(fov)
	{
		_right = front.cross(up);
		_fovScale = tanf(fov * 0.5f * PI / 180) * 2;
	}

	PerspectiveCamera(const PerspectiveCamera&) = delete;
	PerspectiveCamera& operator=(const PerspectiveCamera&) = delete;

	Ray generate_ray(float x, float y)
	{
		auto r = _right * ((x - 0.5) * _fovScale);
		auto u = _up * ((y - 0.5) * _fovScale);
		auto d = _front + r + u;
		d.normalize();
		return Ray(_eye, d);
	}

};
