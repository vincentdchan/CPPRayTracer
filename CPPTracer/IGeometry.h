#pragma once
#include "IMaterial.h"

using namespace Material;

namespace Shape
{

	class IGeometry
	{
	public:
		virtual IMaterial* get_material() const = 0;

	};

}
