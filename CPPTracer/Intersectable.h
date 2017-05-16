#pragma once
#include <memory>
#include "Ray.h"

namespace Shape
{

	class IntersectResult;

	class Intersectable
	{
	public:

		virtual bool intersect(const Ray&, IntersectResult& result) const = 0;
		virtual ~Intersectable() 
		{}

	};

}
