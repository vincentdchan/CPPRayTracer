#include "PointLight.h"
#include "IntersectResult.h"

namespace Light
{

	PointLight::PointLight()
	{}

	PointLight::PointLight(const Color& intensity, const Vector3f& position):
		_intensity(intensity), _position(position), _shadow(true)
	{}

	PointLight::PointLight(const PointLight& pt):
		_intensity(pt._intensity), _position(pt._position), _shadow(pt._shadow)
	{
	}

	PointLight& PointLight::operator=(const PointLight& pt)
	{
		_intensity = pt._intensity;
		_position = pt._position;
		_shadow = pt._shadow;
		return *this;
	}

	bool PointLight::sample(const Intersectable* scene,

		const Vector3f& position,
		LightSample& lightSample) const
	{
		Vector3f delta = _position - position;
		float rr = delta.dot(delta);
		float r = sqrtf(rr);
		Vector3f L = delta / r;

		if (_shadow)
		{
			auto shadowRay = Ray(position, L);
			IntersectResult shadowResult;
			if (scene->intersect(shadowRay, shadowResult))
			{
				if (shadowResult.get_distance() <= r)
					return false;
			}
		}

		float attenuation = 1 / rr;
		lightSample.set_direction(L);
		lightSample.set_irradiance(_intensity * attenuation);
		return true;
	}

}