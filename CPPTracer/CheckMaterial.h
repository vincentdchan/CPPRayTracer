#pragma once
#include "IMaterial.h"
#include <Eigen/Dense>

using namespace Eigen;

namespace Material
{
	class CheckMaterial final : public IMaterial
	{
	private:

		float _scale;
		float _reflectiveness;

	public:
		
		CheckMaterial(float scale, float reflectiveness = 0.0f);
		virtual Color sample(const Ray&, const Vector3f& position, const Vector3f& normal) const;

		inline float get_scale() const { return _scale; }
		inline float get_reflectiveness() const { return _reflectiveness; }

	};
}
