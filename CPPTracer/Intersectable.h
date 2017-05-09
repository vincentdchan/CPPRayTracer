#pragma once
#include "Ray.h"

namespace Shape
{

	class IntersectResult;

	class Intersectable
	{
		virtual IntersectResult intersect(const Ray&) = 0;
	};

}
