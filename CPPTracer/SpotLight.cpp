#include <cmath>
#include "SpotLight.h"
#include "IntersectResult.h"

#define PI 3.141593658

namespace Light
{
	SpotLight::SpotLight() :
		_shadow(true)
	{ }

	SpotLight::SpotLight(const Color& intensity,
		const Vector3f& pos,
		const Vector3f& dir,
		float theta,
		float phi,
		float falloff):
		_intensity(intensity), _position(pos), _direction(dir),
		_theta(theta), _phi(phi), _falloff(falloff), _shadow(true)
	{
		initialize();
	}

	SpotLight::SpotLight(const SpotLight& that):
		_intensity(that._intensity),
		_position(that._position),
		_direction(that._direction),
		_theta(that._theta), _phi(that._phi), _falloff(that._falloff),
		_shadow(that._shadow), S(that.S), cos_theta(that.cos_theta), 
		cos_phi(that.cos_phi), base_multiplier(that.base_multiplier)
	{ }

	SpotLight& SpotLight::operator=(const SpotLight& that)
	{
		_intensity = that._intensity;
		_position = that._position;
		_direction = that._direction;
		_theta = that._theta;
		_phi = that._phi;
		_falloff = that._falloff;
		_shadow = that._shadow;

		S = that.S;
		cos_theta = that.cos_theta;
		cos_phi = that.cos_phi;
		base_multiplier = that.base_multiplier;
		return *this;
	}

	void SpotLight::initialize()
	{
		S = _direction;
		S.normalize();
		S *= -1;
		cos_theta = cosf(_theta / PI / 180 / 2);
		cos_phi = cosf(_phi / PI / 180 / 2);
		float tmp = cos_theta - cos_phi;
		base_multiplier = tmp == 0? tmp : 1 / tmp;
	}

	bool SpotLight::sample(const Intersectable* scene,
		const Vector3f& position,
		LightSample& lightSample) const
	{
		Vector3f delta= _position - position;
		float rr = delta.dot(delta);
		float r = sqrtf(rr);
		Vector3f L = delta / r;

		float spot;
		float sl = S.dot(L);
		if (sl >= cos_theta)
			spot = 1;
		else if (sl <= cos_phi)
			spot = 0;
		else
			spot = powf((sl - cos_phi) * base_multiplier, _falloff);

		if (_shadow)
		{
			Ray ray(position, L);
			IntersectResult shadowResult;
			if (scene->intersect(ray, shadowResult))
				if (shadowResult.get_distance() <= r)
					return false;
		}

		float attenuation = 1 / rr;

		lightSample.set_direction(L);
		lightSample.set_irradiance(_intensity * (attenuation * spot));
		return true;
	}

}
