#pragma once
#include <Eigen/Dense>
#include "ILight.h"
#include "Color.h"

using namespace Eigen;

namespace Light
{

	class SpotLight : public ILight
	{
	private:
		Color _intensity;
		Vector3f _position;
		Vector3f _direction;
		float _theta;
		float _phi;
		float _falloff;
		bool _shadow;

		Vector3f S;
		float cos_theta;
		float cos_phi;
		float base_multiplier;

		void initialize();

	public:
		SpotLight();
		SpotLight(const Color&, const Vector3f&, const Vector3f&,
			float theta, float phi, float falloff);
		SpotLight(const SpotLight&);

		SpotLight& operator=(const SpotLight&);

		bool sample(const Intersectable* scene, 
			const Vector3f& position, 
			LightSample& lightSample) const override;

		inline const Color& GetIntensity() const { return _intensity; }
		inline const Vector3f& GetPosition() const { return _position; }
		inline const Vector3f& GetDirection() const { return _direction; }
		inline float GetTheta() const { return _theta; }
		inline float GetPhi() const { return _phi; }
		inline float GetFallOff() const { return _falloff; }

		void SetIntensity(const Color& color)
		{
			_intensity = color;
			initialize();
		}

		void SetPositoin(const Vector3f& position)
		{
			_position = position;
			initialize();
		}

		void SetDirection(const Vector3f& vec)
		{
			_direction = vec;
			initialize();
		}

		void SetTheta(float f)
		{
			_theta = f;
			initialize();
		}

		void SetPhi(float f)
		{
			_phi = f;
			initialize();
		}

		void SetFallOff(float f)
		{
			_falloff = f;
			initialize();
		}

	};

}
