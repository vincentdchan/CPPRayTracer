#pragma once
#include <functional>
#include <Eigen/Dense>
#include <queue>
#include <memory>
#include <vector>
#include <algorithm>
#include <boost/thread/thread.hpp>

#include "Mutex.h"
#include "Intersectable.h"
#include "PerspectiveCamera.h"
#include "Tile.h"
#include "Bound.h"
#include "Color.h"

using namespace Eigen;

class RayTracer
{
public:
	typedef std::function<void (int)> UpdateFunction;
	static const int tileWidth = 16;
	static const int tileHeight = 16;
	static const int threadCount = 8;

private:
	unsigned char* _data;
	int _width;
	int _height;
	int _maxReflect;
	std::shared_ptr<Tile> _tile;
	std::shared_ptr<Shape::Intersectable> _scene;
	std::shared_ptr<PerspectiveCamera> _camera;
	std::unique_ptr<std::queue<Bound>> _queue;
	UpdateFunction _updateFunc;

	boost::mutex queue_mtx_;
	boost::mutex merge_mtx_;

	void render_thread_async();
	bool render_tile_from_queue();

public:

	RayTracer():
		_data(nullptr), _maxReflect(3)
	{}

	void run();
	void parallel_run();
	void renderTile(const Bound&, Tile&);
	std::unique_ptr<std::queue<Bound>> 
		get_tile_bounds_queue() const;

	inline const unsigned char * 
		get_date() const { return _data; }

	inline int get_width() const { return _width; }

	inline void set_width(int width) { _width = width; }

	inline int get_height() const { return _height; }

	inline void set_height(int height) { _height = height; }

	inline const std::shared_ptr<Shape::Intersectable>&
		get_scene() const { return _scene; }

	inline void set_scene(const std::shared_ptr<Shape::Intersectable>& ptr) { _scene = ptr; }

	inline const std::shared_ptr<Tile>&
		get_tile() const { return _tile; }

	inline std::shared_ptr<PerspectiveCamera>
		get_camera() const { return _camera; }

	inline void set_camera(const std::shared_ptr<PerspectiveCamera>& ptr) { _camera = ptr; }

	inline void set_update_callback(UpdateFunction func) { _updateFunc = func; }

	~RayTracer()
	{
		if (_data != nullptr)
			delete[] _data;
	}

	static void renderDepth(unsigned char** ptr, int width, int height, 
		const Shape::Intersectable&, 
		const PerspectiveCamera&, int maxDepth, UpdateFunction func);

	static void renderMaterial(unsigned char** ptr, int width, int height, 
		const Shape::Intersectable&, 
		const PerspectiveCamera&, UpdateFunction func);

	static Color 
		rayTraceRecursive(const Shape::Intersectable& scene, const Ray& ray, int maxReflect);

	static void renderReflection(unsigned char** ptr, int width, int height, 
		const Shape::Intersectable&, 
		const PerspectiveCamera&, 
		int maxReflect,
		UpdateFunction func);

};
