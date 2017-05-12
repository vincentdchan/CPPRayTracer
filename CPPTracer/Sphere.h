#pragma once
#include "Intersectable.h"
#include "IntersectResult.h"
#include <Eigen/Dense>

namespace Shape
{
	using namespace Eigen;

	class Sphere final : public Intersectable
	{
	private:
		Vector3f _center;
		float _radius;
		float _sqrRadius;

	public:

		Sphere(Vector3f center, float radius):
			_center(center), _radius(radius), _sqrRadius(radius * radius)
		{}

		Sphere(const Sphere& that):
			_center(that._center), _radius(that._radius), _sqrRadius(that._sqrRadius)
		{}

		virtual IntersectResult intersect(const Ray& ray) const
		{
			auto v = ray.get_origin() - _center;
			float a0 = v.dot(v) - _sqrRadius;
			auto dv = ray.get_direction().dot(v);

			if (dv <= 0)
			{
				float discr = dv * dv - a0;
				if (discr >= 0)
				{
					float distance = -dv - sqrtf(discr);
					Vector3f position = ray.get_point(distance);
					Vector3f normal = position - _center;
					normal.normalize();
					IntersectResult tmp(this, distance, position, normal);
					return tmp;
				}
			}
			return IntersectResult();
		}

	};

}
