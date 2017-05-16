#pragma once
#include <Eigen/Dense>

#include "IGeometry.h"
#include "Intersectable.h"
#include "IntersectResult.h"
#include "IMaterial.h"

namespace Shape
{

	class Plane : public IGeometry, public Intersectable
	{
	private:
		IMaterial *_material;
		Vector3f _normal;
		Vector3f _position;
		float d;

	public:
		Plane(const Vector3f& normal, float distance) :
			_normal(normal), d(distance)
		{
			_position = _normal * d;
		}

		virtual IMaterial* get_material() const { return _material; }

		virtual bool
			intersect(const Ray& ray, IntersectResult& result) const override
		{
			float a = ray.get_direction().dot(_normal);
			if (a >= 0)
				return false;

			float b = _normal.dot(ray.get_origin() - _position);
			result.set_geometry(this);
			result.set_distance(-b / a);
			result.set_position(ray.get_point(result.get_distance()));
			result.set_normal(_normal);
			return true;
		}

		inline const Vector3f get_normal() const { return _normal; }
		inline float get_distance() const { return d; }

		inline void set_material(IMaterial * material) { _material = material; }

		virtual ~Plane()
		{}

	};

}
