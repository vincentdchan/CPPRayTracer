#pragma once
#include "ILight.h"
#include "Color.h"

namespace Light
{
	class PointLight : public ILight
	{
	private:
		Color _intensity;
		Vector3f _position;
		bool _shadow;

	public:
		PointLight();
		PointLight(const Color&, const Vector3f&);
		PointLight(const PointLight&);

		virtual bool sample(const Intersectable* scene, 
			const Vector3f& position, 
			LightSample& lightSample) const;

		PointLight& operator=(const PointLight&);

		inline const Color& GetIntensity() const
		{
			return _intensity;
		}

		inline void SetIntensity(const Color& color)
		{
			_intensity = color;
		}

		inline const Vector3f& GetPosition() const
		{
			return _position;
		}

		inline void SetPosition(const Vector3f& vec)
		{
			_position = vec;
		}

	};
}
