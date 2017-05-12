#pragma once
#include <Windows.h>
#include "App.h"
#include "Intersectable.h"
#include "PerspectiveCamera.h"

#include <d2d1.h>

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

private:
	HANDLE renderThreadHandle;
	DWORD renderThreadId;
	DWORD uiThreadId;

	char* renderDepth(int width, int height, const Shape::Intersectable&, const PerspectiveCamera&, int maxDepth);
	char* _renderedPixels;
	ID2D1Bitmap * _renderedBitmap;

	static DWORD WINAPI RenderThreadFunc(LPVOID lpParam);

protected:

	virtual HRESULT OnRender();
	virtual void DiscardDeviceResources();
	virtual HRESULT OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};
