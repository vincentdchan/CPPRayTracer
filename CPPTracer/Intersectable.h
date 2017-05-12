#pragma once
#include "Ray.h"

namespace Shape
{

	class IntersectResult;

	class Intersectable
	{
	public:
		virtual IntersectResult intersect(const Ray&) const = 0;
	};

}
