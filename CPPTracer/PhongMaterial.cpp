#include <Eigen/Dense>
#include <math.h>
#include <algorithm>
#include "PhongMaterial.h"

using namespace Eigen;

namespace Material
{
	PhongMaterial::PhongMaterial(const Color& diffuse, 
		const Color& specular, 
		float shiniess, 
		float reflectiveness):
		_diffuse(diffuse), _specular(specular), _shiniess(shiniess), _reflectiveness(reflectiveness)
	{
		lightDir = Vector3f(1, 1, 1);
		lightDir.normalize();
		lightColor = Color::white;
	}

	Color PhongMaterial::sample(const Ray& ray, 
		const Vector3f& position, 
		const Vector3f& normal) const
	{
		float nl = normal.dot(lightDir);
		Vector3f H = lightDir - ray.get_direction();
		H.normalize();
		float nh = normal.dot(H);
		Color diffuseTerm = _diffuse * std::max(nl, 0.0f);
		Color specularTerm = _specular * powf(std::max(nh, 0.0f), _shiniess);
		return lightColor.modulate(diffuseTerm + specularTerm);
	}

}