#pragma once
#include <Eigen/Dense>
#include "Intersectable.h"

using namespace Eigen;

namespace Shape
{

	class IntersectResult final
	{

	private:
		Intersectable * _geometry;
		float _distance;
		Vector3f _position;
		Vector3f _normal;

	public:

		IntersectResult():
			_geometry(nullptr), _distance(0)
		{
		}

		IntersectResult(Intersectable * geometry, float distance, const Vector3f& position, const Vector3f& normal):

			_geometry(geometry),
			_distance(distance),
			_position(position),
			_normal(normal)
		{}

		IntersectResult(const IntersectResult& that):
			_geometry(that._geometry), 
			_distance(that._distance),
			_position(that._position),
			_normal(that._normal)
		{}

		IntersectResult& operator=(const IntersectResult&) = delete;

		inline Intersectable * get_geometry()
		{
			return _geometry;
		}

		inline float get_distance()
		{
			return _distance;
		}

		inline const Vector3f& get_position()
		{
			return _position;
		}

		inline const Vector3f& get_normal()
		{
			return _normal;
		}

	};

}
