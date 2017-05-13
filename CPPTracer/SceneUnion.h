#pragma once
#include <vector>
#include "Intersectable.h"
#include "IntersectResult.h"
#include "IGeometry.h"

using namespace Shape;

class SceneUnion : 
	public std::vector<Intersectable*>, public Intersectable
{

	virtual std::shared_ptr<IntersectResult> 
		intersect(const Ray&) const;

};
