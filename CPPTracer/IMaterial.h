#pragma once
#include <Eigen/Dense>

#include "Ray.h"
#include "Color.h"

using namespace Eigen;

namespace Material 
{

	class IMaterial
	{
	public:

		virtual Color sample(const Ray&, const Vector3f& position, const Vector3f& normal) const = 0;

	};

}

