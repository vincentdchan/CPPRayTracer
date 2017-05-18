#pragma once
#include <Eigen/Dense>
#include <memory>

#include "ILight.h"
#include "Color.h"

namespace Light
{
	using namespace Eigen;

	class DirectionalLight : public ILight
	{
	private:
		Color _irradiance;
		// Vector3f _direction;
		Vector3f _real_direction;
		bool _shadow;

	public:

		DirectionalLight(): _irradiance(Color::black), _shadow(true) 
		{}

		DirectionalLight(const Color& irradiance, const Vector3f& direction):
			_irradiance(irradiance), /*_direction(direction),*/ _shadow(true)
		{
			_real_direction = direction;
			_real_direction.normalize();
			_real_direction *= -1;
		}

		bool
		sample(const Intersectable* scene, 
			const Vector3f& position,
			LightSample& lightSample) const override;

		inline const Color& get_irradiance() const { return _irradiance; }

		// inline const Vector3f& get_direction() const { return _direction; }

		inline bool get_shadow() const { return _shadow; }

	};

}
