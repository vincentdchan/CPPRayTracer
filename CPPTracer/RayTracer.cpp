#include "RayTracer.h"
#include "IntersectResult.h"
#include <algorithm>

void RayTracer::renderDepth(unsigned char** ptr, int width, int height,
	const Shape::Intersectable& scene,
	const PerspectiveCamera& camera, int maxDepth, UpdateFunction func)
{
	*ptr = new unsigned char[width * height * 4];
	unsigned char *pixels = *ptr;

	int i = 0;
	Shape::IntersectResult result;
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			if (scene.intersect(ray, result))
			{
				char depth = 255 - std::min<int>((result.get_distance() / maxDepth) * 255, 255);
				pixels[i++] = (result.get_normal()(2) + 1) * 128;
				pixels[i++] = (result.get_normal()(1) + 1) * 128;
				pixels[i++] = (result.get_normal()(0) + 1) * 128;
				pixels[i++] = 255;
			}
			else
			{
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = static_cast<char>(255);
			}
		}
		func(y);
	}
}

void RayTracer::renderMaterial(unsigned char** ptr, int width, int height,
	const Shape::Intersectable& scene,
	const PerspectiveCamera& camera, UpdateFunction func)
{
	unsigned char* pixels = new unsigned char[width * height * 4];
	*ptr = pixels;

	int i = 0;
	Shape::IntersectResult result;
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			if (scene.intersect(ray, result))
			{
				auto color = result.get_geometry()->get_material()->sample(ray, result.get_position(), result.get_normal());

				pixels[i++] = std::min<int>(color(2) * 255, 255);
				pixels[i++] = std::min<int>(color(1) * 255, 255);
				pixels[i++] = std::min<int>(color(0) * 255, 255);
				pixels[i++] = 255;
			}
			else
			{
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = static_cast<char>(255);
			}
		}
		func(y);
	}
}

Color 
RayTracer::rayTraceRecursive(const Shape::Intersectable& scene, const Ray& ray, int maxReflect)
{
	Shape::IntersectResult result;
	if (scene.intersect(ray, result))
	{
		auto reflectiveness = result.get_geometry()->get_material()->get_reflectiveness();
		Color color = result.get_geometry()->get_material()->sample(ray, 
			result.get_position(), result.get_normal());
		color = color * (1.0f - reflectiveness);

		if (reflectiveness > 0 && maxReflect > 0)
		{
			Vector3f r = (result.get_normal() * (-2 * result.get_normal().dot(ray.get_direction()))) + 
				ray.get_direction();
			Ray new_ray(result.get_position(), r);
			Color reflectedColor = rayTraceRecursive(scene, new_ray, maxReflect - 1);
			color = color + (reflectedColor * reflectiveness);
		}
		return color;
	}
	else
		return Color::black;
}

void RayTracer::renderReflection(unsigned char** ptr, int width, int height,
	const Shape::Intersectable& scene,
	const PerspectiveCamera& camera,
	int maxReflect,
	UpdateFunction func)
{
	unsigned char* pixels = new unsigned char[width * height * 4];
	*ptr = pixels;

	int i = 0;
	int delta = std::max<int>(height / 60, 10);
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			auto color = rayTraceRecursive(scene, ray, maxReflect);
			pixels[i++] = std::min<int>(color(2) * 255, 255);
			pixels[i++] = std::min<int>(color(1) * 255, 255);
			pixels[i++] = std::min<int>(color(0) * 255, 255);
			pixels[i++] = 255;
		}
		func(y);
	}
}
