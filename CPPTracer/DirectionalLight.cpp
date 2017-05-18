#include "DirectionalLight.h"
#include "IntersectResult.h"
#include "Ray.h"

namespace Light
{

	bool
		DirectionalLight::sample(
			const Intersectable* scene, 
			const Vector3f& position,
			LightSample& lightSample) const
	{

		IntersectResult shadowResult;
		if (_shadow)
		{
			Ray shadowRay(position, _real_direction);
			if (scene->intersect(shadowRay, shadowResult))
				return false;
		}

		lightSample.set_direction(_real_direction);
		lightSample.set_irradiance(_irradiance);
		return true;
	}

}