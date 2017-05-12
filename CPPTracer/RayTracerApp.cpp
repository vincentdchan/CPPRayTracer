#include "RayTracerApp.h"
#include "IntersectResult.h"

#include "Sphere.h"
#include "PerspectiveCamera.h"

#include <Eigen/Dense>

using namespace Eigen;

char* RayTracerApp::renderDepth(
	int width, int height, 
	const Shape::Intersectable& scene, const PerspectiveCamera& camera, int maxDepth)
{
	char* pixels = new char[width * height * 4];

	int i = 0;
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			auto result = scene.intersect(ray);
			if (result.get_geometry() != nullptr)
			{
				char depth = 255 - static_cast<char>(min((result.get_distance() / maxDepth) * 255, 255));
				pixels[i++] = depth;
				pixels[i++] = depth;
				pixels[i++] = depth;
				pixels[i++] = static_cast<char>(255);
			}
			else
			{
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = static_cast<char>(255);
			}
		}
	}

	return pixels;
}

HRESULT RayTracerApp::Initialize()
{
	HRESULT hr = App::Initialize();

	using namespace Shape;
	Sphere scene(Vector3f(0, 10, -10), 10);
	PerspectiveCamera camera(
		Vector3f(0, 10, 10),
		Vector3f(0, 0, -1),
		Vector3f(0, 1, 0),
		90
	);
	_renderedPixels = renderDepth(400, 400, scene, camera, 20);
	this->OnRender();
	return hr;
}

HRESULT RayTracerApp::OnRender()
{
	HRESULT hr = S_OK;

	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat();
	pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	D2D1_BITMAP_PROPERTIES prop = D2D1::BitmapProperties();
	prop.pixelFormat = pixelFormat;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		if (_renderedPixels != nullptr)
		{
			hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(400, 400), _renderedPixels, 400 * 4, &prop, &_renderedBitmap);
			// hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(400, 400), prop, &_renderedBitmap);

			if (SUCCEEDED(hr))
			{
				 m_pRenderTarget->DrawBitmap(_renderedBitmap, D2D1::RectF(0, 0, 400, 400), 1, 
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					D2D1::RectF(0, 0, 400, 400));
				 SafeRelease(&_renderedBitmap);
			}

		}


		hr = m_pRenderTarget->EndDraw();

	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

void RayTracerApp::DiscardDeviceResources()
{
	App::DiscardDeviceResources();
	SafeRelease(&_renderedBitmap);
}

RayTracerApp::~RayTracerApp()
{
	SafeRelease(&_renderedBitmap);
	delete[] _renderedPixels;
}
