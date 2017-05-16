#pragma once
#include <vector>
#include <memory>
#include "Intersectable.h"
#include "IntersectResult.h"
#include "IGeometry.h"

using namespace Shape;

class SceneUnion : 
	public std::vector<std::unique_ptr<Intersectable>>, 
	public Intersectable
{

	virtual bool intersect(const Ray&, IntersectResult&) const override;

};
