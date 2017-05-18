#include "LightRayTracer.h"
#include "IntersectResult.h"

Color LightRayTracer::RenderPixel(float sx, float sy) const
{
	using namespace Shape;
	auto ray = _camera->generate_ray(sx, sy);
	IntersectResult result;
	Light::LightSample lightSample;
	if (_scene->intersect(ray, result))
	{
		Color color = Color::black;
		for (auto i = lights_->begin(); i != lights_->end(); ++i)
		{
			if ((*i)->sample(_scene.get(), result.get_position(), lightSample))
			{
				float nl = result.get_normal().dot(lightSample.get_direction());

				if (nl >= 0)
					color += (lightSample.get_irradiance() * nl);
			}
		}
		return color;
	}
	else
		return Color::black;
}
