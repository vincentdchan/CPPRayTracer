#pragma once
#include <Eigen/Dense>
#include "IGeometry.h"
#include "Intersectable.h"

using namespace Eigen;

namespace Shape
{

	class IntersectResult final
	{

	private:
		const IGeometry * _geometry;
		float _distance;
		Vector3f _position;
		Vector3f _normal;

	public:

		static const IntersectResult nullResult;

		IntersectResult():
			_geometry(nullptr), _distance(0)
		{
		}

		IntersectResult(const IGeometry * geometry, float distance, const Vector3f& position, const Vector3f& normal):

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

		IntersectResult& operator=(const IntersectResult& that)
		{
			_geometry = that._geometry;
			_distance = that._distance;
			_position = that._position;
			_normal = that._normal;
			return *this;
		}

		inline const IGeometry * get_geometry() const
		{
			return _geometry;
		}

		inline void set_geometry(const IGeometry * geo)
		{
			_geometry = geo;
		}

		inline float get_distance() const
		{
			return _distance;
		}

		inline void set_distance(float distance)
		{
			_distance = distance;
		}

		inline const Vector3f& get_position() const
		{
			return _position;
		}

		inline void set_position(const Vector3f& vec)
		{
			_position = vec;
		}

		inline const Vector3f& get_normal() const
		{
			return _normal;
		}

		inline void set_normal(const Vector3f& nor)
		{
			_normal = nor;
		}

	};

}
