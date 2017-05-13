#pragma once
#include "Color.h"
#include "IMaterial.h"

namespace Material
{

	class PhongMaterial final : public IMaterial
	{
	private:
		Color _diffuse;
		Color _specular;
		float _shiniess;
		float _reflectiveness;

	public:
		Vector3f lightDir;
		Color lightColor;

		PhongMaterial(const Color&, const Color&, float shiniess, float reflectiveness = 0.0f);

		virtual Color sample(const Ray&, const Vector3f& position, const Vector3f& normal) const;

		inline const Color& get_diffuse() const { return _diffuse; }
		inline const Color& get_specular() const { return _specular; }
		inline float get_shiniess() const { return _shiniess; }
		virtual float get_reflectiveness() const { return _reflectiveness; }

	};

}
