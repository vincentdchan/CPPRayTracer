#pragma once
#include <Windows.h>
#include "App.h"
#include "Intersectable.h"
#include "PerspectiveCamera.h"
#include "Ray.h"
#include "Color.h"

#include <d2d1.h>
#include <ctime>

#define MSG_REPAINT WM_USER

/// <summary>
/// A double-thread ray tracer renderer
/// </summay>
class RayTracerApp final : public App {
public:

	virtual HRESULT Initialize();
	virtual ~RayTracerApp();

	RayTracerApp() : 
		App(), _renderedBitmap(nullptr), _renderedPixels(nullptr)
	{
	}

	static const int renderWidth = 1024;
	static const int renderHeight = 1024;

private:
	HANDLE renderThreadHandle;
	DWORD renderThreadId;
	DWORD uiThreadId;
	float timeDiff;

	ID2D1SolidColorBrush *m_pBlackBrush;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	Color 
		rayTraceRecursive(const Shape::Intersectable& scene, const Ray& ray, int maxReflect);

	char* renderDepth(int width, int height, const Shape::Intersectable&, const PerspectiveCamera&, int maxDepth);
	void renderMaterial(unsigned char** ptr, int width, int height, 
		const Shape::Intersectable&, 
		const PerspectiveCamera&);
	void renderReflection(unsigned char** ptr, int width, int height, 
		const Shape::Intersectable&, 
		const PerspectiveCamera&, int maxReflet);

	void saveToFile(const char* filename, unsigned char *pixels, int srcWidth, int srcHeight);

	unsigned char* _renderedPixels;
	ID2D1Bitmap * _renderedBitmap;

	static DWORD WINAPI RenderThreadFunc(LPVOID lpParam);

protected:

	virtual HRESULT OnRender() override;
	virtual void DiscardDeviceResources() override;
	virtual HRESULT OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual HRESULT CreateDeviceIndependentResources() override;
	virtual HRESULT CreateDeviceResources() override;

};
