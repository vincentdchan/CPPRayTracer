#pragma once
#include <Eigen/Dense>
#include <memory>

#include "Intersectable.h"
#include "IntersectResult.h"
#include "IGeometry.h"

namespace Shape
{
	using namespace Eigen;

	class Sphere final : public Intersectable, public IGeometry
	{
	private:
		Vector3f _center;
		float _radius;
		float _sqrRadius;

		IMaterial * _material;

	public:

		Sphere(Vector3f center, float radius):
			_center(center), _radius(radius), _sqrRadius(radius * radius), _material(nullptr)
		{}

		Sphere(const Sphere& that):
			_center(that._center), _radius(that._radius), _sqrRadius(that._sqrRadius), 
			_material(that._material)
		{}

		virtual std::shared_ptr<IntersectResult> 
			intersect(const Ray& ray) const
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
					return std::make_shared<IntersectResult>(
						this, distance, position, normal);
				}
			}
			return std::make_shared<IntersectResult>(IntersectResult::nullResult);
		}

		void set_material(IMaterial * material) {
			_material = material;
		}

		virtual IMaterial* get_material() const {
			return _material;
		}

	};

}
