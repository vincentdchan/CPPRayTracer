#pragma once
#include <memory>
#include "Ray.h"

namespace Shape
{

	class IntersectResult;

	class Intersectable
	{
	public:
		virtual std::shared_ptr<IntersectResult> 
			intersect(const Ray&) const = 0;
	};

}
