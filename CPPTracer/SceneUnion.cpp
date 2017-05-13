#include <limits>
#include "SceneUnion.h"

std::shared_ptr<IntersectResult> 
SceneUnion::intersect(const Ray& ray) const
{
	float minDistance = std::numeric_limits<float>::max();
	auto minResult = std::make_shared<IntersectResult>(IntersectResult::nullResult);
	for (auto i = begin(); i != end(); ++i)
	{
		auto result = (*i)->intersect(ray);
		if (result->get_geometry() && result->get_distance() < minDistance)
		{
			minDistance = result->get_distance();
			minResult = result;
		}
	}
	return minResult;
}
