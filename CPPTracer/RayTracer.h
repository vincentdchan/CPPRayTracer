#pragma once
#include "Intersectable.h"
#include "PerspectiveCamera.h"
#include "Color.h"
#include <functional>

class RayTracer
{
public:
	typedef std::function<void (int)> UpdateFunction;

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
