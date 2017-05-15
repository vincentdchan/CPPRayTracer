#include "DirectionalLight.h"
#include "IntersectResult.h"
#include "Ray.h"

namespace Light
{

	std::shared_ptr<LightSample> 
		DirectionalLight::sample(const Intersectable* scene, const Vector3f& position) const
	{

		IntersectResult shadowResult;
		if (_shadow)
		{
			Ray shadowRay(position, _real_direction);
			if (scene->intersect(shadowRay, shadowResult))
				return std::make_shared<LightSample>(LightSample::zero);
		}

		return std::make_shared<LightSample>(_real_direction, _irradiance);
	}

}