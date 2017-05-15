#include <limits>
#include "SceneUnion.h"

bool SceneUnion::intersect(const Ray& ray, IntersectResult& result) const
{
	bool allresult = false;
	float minDistance = std::numeric_limits<float>::max();

	for (auto i = begin(); i != end(); ++i)
	{
		auto bResult = (*i)->intersect(ray, result);
		if (bResult && result.get_distance() < minDistance)
		{
			allresult = true;
			minDistance = result.get_distance();
		}
	}
	return allresult;
}
