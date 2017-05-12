#pragma once
#include <Windows.h>
#include "App.h"
#include "Intersectable.h"
#include "PerspectiveCamera.h"

#include <d2d1.h>

class RayTracerApp final : public App {
public:

	virtual HRESULT Initialize();
	virtual ~RayTracerApp();

	RayTracerApp() : 
		App(), _renderedBitmap(nullptr), _renderedPixels(nullptr)
	{
	}

private:


	char* renderDepth(int width, int height, const Shape::Intersectable&, const PerspectiveCamera&, int maxDepth);
	char* _renderedPixels;
	ID2D1Bitmap * _renderedBitmap;

protected:

	virtual HRESULT OnRender();
	virtual void DiscardDeviceResources();

};
