#pragma once

#include <Eigen/Dense>
#include <memory>

#include "Intersectable.h"
#include "LightSample.h"

namespace Light
{

	using namespace Eigen;
	using namespace Shape;

	class ILight
	{
	public:

		virtual std::shared_ptr<LightSample> 
		sample(const Intersectable* scene, const Vector3f& position) const = 0;

	};

}
