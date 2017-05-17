#include "RayTracer.h"
#include "IntersectResult.h"
#include <algorithm>

void RayTracer::run()
{
	_tile = std::make_shared<Tile>(_width, _height);

	int i = 0;
	int delta = std::max<int>(_height / 60, 10);
	for (int y = 0; y < _height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / _height;
		for (int x = 0; x < _width; ++x)
		{
			float sx = static_cast<float>(x) / _width;
			auto ray = _camera->generate_ray(sx, sy);
			auto color = rayTraceRecursive(*_scene, ray, _maxReflect);
			_tile->push_color(color);
		}
		_updateFunc(y);
	}
	_updateFunc(_height);
}

void RayTracer::parallel_run()
{
	_tile = std::make_shared<Tile>(_width, _height);

	_queue = get_tile_bounds_queue();

	std::vector<boost::thread> threads;
	for (int i = 0; i < threadCount; ++i)
	{
		threads.push_back(boost::thread(boost::bind(&RayTracer::render_thread_async, this)));
	}

	for (auto i = threads.begin(); i != threads.end(); ++i)
	{
		i->join();
	}

}

#undef min
std::unique_ptr<std::queue<Bound>>
RayTracer::get_tile_bounds_queue() const
{
	auto result = std::make_unique<std::queue<Bound>>();
	for (int y = 0; y < _height; y += tileHeight)
	{
		for (int x = 0; x < _width; x += tileWidth)
		{
			Vector2i leftTop(x, y);
			Vector2i rightBottom(
				std::min(x + tileWidth, _width), 
				std::min(y + tileHeight, _height)
			);
			result->push(Bound(leftTop, rightBottom));
		}
	}
	return result;
}

void RayTracer::renderTile(const Bound& bound, Tile& tile)
{
	int i = 0;
	int width = bound.get_width();
	int height = bound.get_height();
	int delta = std::max<int>(height / 60, 10);
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y + bound.get_left_top()(1)) / _height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x + bound.get_left_top()(0)) / _width;
			auto ray = _camera->generate_ray(sx, sy);
			auto color = rayTraceRecursive(*_scene, ray, _maxReflect);
			tile.push_color(color);
		}
	}
}

void RayTracer::render_thread_async()
{
	while (render_tile_from_queue());
}

bool RayTracer::render_tile_from_queue()
{
	using namespace Util;

	Bound bound;

	queue_mtx_.lock();
	if (_queue->empty())
	{
		queue_mtx_.unlock();
		return false;
	}
	bound = _queue->front();
	_queue->pop();
	queue_mtx_.unlock();

	Tile tile(bound.get_width(), bound.get_height());
	renderTile(bound, tile);

	merge_mtx_.lock();
	_tile->merge(bound, tile);
	_updateFunc(_height);
	merge_mtx_.unlock();

	return true;
}

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
