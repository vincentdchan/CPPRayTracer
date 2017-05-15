#pragma once
#include <Eigen/Dense>

#include "Color.h"

namespace Light
{

	using namespace Eigen;

	class LightSample final 
	{
	private:
		Vector3f _direction;
		Color _irradiance;

	public:

		static const LightSample zero;

		LightSample():
			_irradiance(Color::black)
		{}

		LightSample(const Vector3f& direction, const Color& irradiance) :
			_direction(direction), _irradiance(irradiance)
		{}

		inline const Vector3f& get_direction() const
		{
			return _direction;
		}

		inline const Color& get_irradiance() const
		{
			return _irradiance;
		}

	};

}
