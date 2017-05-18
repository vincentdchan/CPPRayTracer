#pragma once
#include <Windows.h>
#include <boost/thread/thread.hpp>
#include "App.h"
#include "RayTracer.h"
#include "Intersectable.h"
#include "PerspectiveCamera.h"
#include "Ray.h"
#include "Color.h"
#include "Tile.h"

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
		App(), _renderedBitmap(nullptr)
	{
	}

	static const int renderWidth = 1024;
	static const int renderHeight = 1024;

private:
	boost::mutex update_mtx_;
	float timeDiff;

	ID2D1SolidColorBrush *m_pBlackBrush;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	std::shared_ptr<Tile> _renderedTile;
	std::shared_ptr<RayTracer> _rayTracer;
	ID2D1Bitmap * _renderedBitmap;

	void SaveToFile(const char* filename, unsigned char *pixels, int srcWidth, int srcHeight);
	void RenderThread();

protected:

	virtual HRESULT OnRender() override;
	virtual void DiscardDeviceResources() override;
	virtual HRESULT OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual HRESULT CreateDeviceIndependentResources() override;
	virtual HRESULT CreateDeviceResources() override;

};
