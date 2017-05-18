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

		LightSample(const LightSample& ls):
			_direction(ls._direction), _irradiance(_irradiance)
		{}

		LightSample(const Vector3f& direction, const Color& irradiance) :
			_direction(direction), _irradiance(irradiance)
		{}

		LightSample& operator=(const LightSample& lightSample)
		{
			_direction = lightSample._direction;
			_irradiance = lightSample._irradiance;
		}

		inline void set_direction(const Vector3f& vec)
		{
			_direction = vec;
		}

		inline const Vector3f& get_direction() const
		{
			return _direction;
		}

		inline void set_irradiance(const Color& color)
		{
			_irradiance = color;
		}

		inline const Color& get_irradiance() const
		{
			return _irradiance;
		}

	};

}
