#pragma once
#include <vector>
#include <memory>
#include "RayTracer.h"
#include "Color.h"
#include "ILight.h"

class LightRayTracer : public RayTracer
{
public:
	typedef std::shared_ptr<std::vector<std::unique_ptr<Light::ILight>>> PLightArray;

private:
	PLightArray lights_;

public:
	virtual Color RenderPixel(float sx, float sy) const override;

	inline PLightArray GetLightCollection() const
	{
		return lights_;
	}

	inline void SetLightCollection(const PLightArray& lights)
	{
		lights_ = lights;
	}

};
