#include <math.h>
#include "CheckerMaterial.h"

namespace Material
{

	CheckerMaterial::CheckerMaterial(float scale, float reflectiveness):
		_scale(scale), _reflectiveness(reflectiveness)
	{}
		
	Color CheckerMaterial::sample(const Ray& ray, 
		const Vector3f& position, 
		const Vector3f& normal) const
	{
		return fabsf(
			fmodf(
				floorf(position(0) * 0.1) + floorf(position(2) * _scale), 2)) < 1 ? 
			Color::black : Color::white;
	}

}