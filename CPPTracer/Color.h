#pragma once

#include <Eigen/Dense>

using namespace Eigen;

class Color final : public Vector3f
{
public:

	static const Color black;
	static const Color white;
	static const Color red;
	static const Color green;
	static const Color blue;

	Color(): Color(0, 0, 0)
	{}

	Color(float r, float g, float b) :
		Vector3f(r, g, b)
	{}

	inline Color modulate(const Vector3f& that) const
	{
		const Color& T = *this;
		return Color(T(0) * that(0), T(1) * that(1), T(2) * that(2));
	}

	inline Color operator+(const Color& that) const
	{
		return Color((*this)(0) + that(0), 
			(*this)(1) + that(1), 
			(*this)(2) + that(2));
	}

	inline Color operator+(float f) const
	{
		return Color((*this)(0) + f, 
			(*this)(1) + f, 
			(*this)(2) + f);
	}

	inline Color operator+(int f) const
	{
		return Color((*this)(0) + f, 
			(*this)(1) + f, 
			(*this)(2) + f);
	}

	inline Color operator*(float f) const
	{
		return Color((*this)(0) * f, 
			(*this)(1) * f, 
			(*this)(2) * f);
	}

	inline Color operator*(int f) const
	{
		return Color((*this)(0) * f, 
			(*this)(1) * f, 
			(*this)(2) * f);
	}

};
