#include <limits>
#include "SceneUnion.h"

bool SceneUnion::intersect(const Ray& ray, IntersectResult& result) const
{
	bool allresult = false;
	float minDistance = std::numeric_limits<float>::max();
	// 这里必须要有一个tmp对象
	// 因为intersect会污染掉result对象，所以如果直接把result传过去
	// if表达式前面为true而后面为false的话，结果就会不正确
	IntersectResult tmpResult;

	for (auto i = begin(); i != end(); ++i)
	{
		auto bResult = (*i)->intersect(ray, tmpResult);
		if (bResult && tmpResult.get_distance() < minDistance)
		{
			allresult = true;
			minDistance = tmpResult.get_distance();
			result = tmpResult;
		}
	}
	return allresult;
}
