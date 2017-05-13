#include <Eigen/Dense>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

#include "RayTracerApp.h"
#include "IntersectResult.h"

#include "Sphere.h"
#include "Plane.h"
#include "SceneUnion.h"
#include "PhongMaterial.h"
#include "CheckerMaterial.h"
#include "PerspectiveCamera.h"

#include "lodepng.h"

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
			if (result->get_geometry() != nullptr)
			{
				char depth = 255 - static_cast<char>(min((result->get_distance() / maxDepth) * 255, 255));
				pixels[i++] = static_cast<char>((result->get_normal()(2) + 1) * 128);
				pixels[i++] = static_cast<char>((result->get_normal()(1) + 1) * 128);
				pixels[i++] = static_cast<char>((result->get_normal()(0) + 1) * 128);
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

void RayTracerApp::renderMaterial(unsigned char** ptr, int width, int height,
	const Shape::Intersectable& scene,
	const PerspectiveCamera& camera)
{
	unsigned char* pixels = new unsigned char[width * height * 4];
	*ptr = pixels;

	int i = 0;
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			auto result = scene.intersect(ray);
			if (result->get_geometry() != nullptr)
			{
				auto color = result->get_geometry()->get_material()->sample(ray, result->get_position(), result->get_normal());

				pixels[i++] = std::min<int>(color(2) * 255, 255);
				pixels[i++] = std::min<int>(color(1) * 255, 255);
				pixels[i++] = std::min<int>(color(0) * 255, 255);
				pixels[i++] = 255;
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
}

void RayTracerApp::renderReflection(unsigned char** ptr, int width, int height,
	const Shape::Intersectable& scene,
	const PerspectiveCamera& camera,
	int maxReflect)
{
	unsigned char* pixels = new unsigned char[width * height * 4];
	*ptr = pixels;

	int i = 0;
	for (int y = 0; y < height; ++y)
	{
		float sy = 1 - static_cast<float>(y) / height;
		for (int x = 0; x < width; ++x)
		{
			float sx = static_cast<float>(x) / width;
			auto ray = camera.generate_ray(sx, sy);
			auto color = rayTraceRecursive(scene, ray, maxReflect);
			pixels[i++] = std::min<int>(color(2) * 255, 255);
			pixels[i++] = std::min<int>(color(1) * 255, 255);
			pixels[i++] = std::min<int>(color(0) * 255, 255);
			pixels[i++] = 255;
		}
		PostMessage(m_hwnd, WM_USER + 1, 0, 0);
	}
}

/// <summary>
/// ¶þ¶Î¹¹Ôì
/// </summary>
HRESULT RayTracerApp::Initialize()
{
	HRESULT hr = App::Initialize();

	timeDiff = 0;
	if (SUCCEEDED(hr)) {
		uiThreadId = GetCurrentThreadId();
		renderThreadHandle = CreateThread(
			nullptr,
			0,
			RenderThreadFunc,
			this,
			0,
			&renderThreadId
		);
	}

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
			hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(renderWidth, renderHeight), _renderedPixels, renderWidth * 4, &prop, &_renderedBitmap);
			// hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(400, 400), prop, &_renderedBitmap);

			if (SUCCEEDED(hr))
			{
				float ratio = static_cast<float>(renderWidth) / renderHeight;
				D2D1_RECT_F destF;
				if (width > height)
				{
					float destWidth = ratio * height;
					float padding = (width - destWidth) / 2;
					destF = D2D1::RectF(padding, 0, width - padding, height);
				}
				else
				{
					float destHeight = width / ratio;
					float padding = (height - destHeight) / 2;
					destF = D2D1::RectF(0, padding, width, height - padding);
				}

				 m_pRenderTarget->DrawBitmap(_renderedBitmap, destF, 1, 
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					D2D1::RectF(0, 0, renderWidth, renderHeight));
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

Color 
RayTracerApp::rayTraceRecursive(const Shape::Intersectable& scene, const Ray& ray, int maxReflect)
{
	auto result = scene.intersect(ray);

	if (result->get_geometry())
	{
		auto reflectiveness = result->get_geometry()->get_material()->get_reflectiveness();
		Color color = result->get_geometry()->get_material()->sample(ray, 
			result->get_position(), result->get_normal());
		color = color * (1.0f - reflectiveness);

		if (reflectiveness > 0 && maxReflect > 0)
		{
			Vector3f r = (result->get_normal() * (-2 * result->get_normal().dot(ray.get_direction()))) + 
				ray.get_direction();
			Ray new_ray(result->get_position(), r);
			Color reflectedColor = rayTraceRecursive(scene, new_ray, maxReflect - 1);
			color = color + (reflectedColor * reflectiveness);
		}
		return color;
	}
	else
		return Color::black;
}

DWORD WINAPI RayTracerApp::RenderThreadFunc(LPVOID lpParam) {
	RayTracerApp* This = reinterpret_cast<RayTracerApp*>(lpParam);
	using namespace Shape;
	Plane plane(Vector3f(0, 1, 0), 0);
	Sphere sphere1(Vector3f(-10, 10, -10), 10);
	Sphere sphere2(Vector3f(10, 10, -10), 10);
	PhongMaterial phong1(Color(0.85f, 0.1f, 0.1f), Color::white, 16, 0.25);
	PhongMaterial phong2(Color(0.1f, 0.25f, 0.75f), Color::white, 16, 0.25);
	CheckerMaterial check(0.1, 0.5);
	plane.set_material(&check);
	sphere1.set_material(&phong1);
	sphere2.set_material(&phong2);

	SceneUnion _union;
	_union.push_back(&plane);
	_union.push_back(&sphere1);
	_union.push_back(&sphere2);
	PerspectiveCamera camera(
		Vector3f(0, 5, 15),
		Vector3f(0, 0, -1),
		Vector3f(0, 1, 0),
		90
	);
	std::clock_t begin = std::clock();
	This->renderReflection(&(This->_renderedPixels), renderWidth, renderHeight, _union, camera, 3);
	std::clock_t end = std::clock();
	float result_t = (float)(end - begin) / CLOCKS_PER_SEC;
	std::cout << result_t << std::endl;

	// This->renderMaterial(&(This->_renderedPixels), 400, 400, _union, camera);
	// This->OnRender(); // Do not render on this thread,
	// post message to ui thread and make it render
	// MSG msg;
	// bool result = PostThreadMessage(uiThreadId, msg, )
	// bool result = PostThreadMessage(This->uiThreadId, WM_USER, 0, 0);
	bool result =  PostMessage(This->m_hwnd, WM_USER + 1, 0, 0);

	// save to file
	This->saveToFile("file.png", This->_renderedPixels, renderWidth, renderHeight);
	return result ? 0 : 1;
}

void RayTracerApp::saveToFile(const char* filename, unsigned char *pixels, int srcWidth, int srcHeight) 
{ 
	int maxLen = srcWidth * srcHeight * 4;

	std::vector<unsigned char> vec;
	for (int i = 0; i < maxLen; i += 4)
	{
		vec.push_back(pixels[i + 2]);
		vec.push_back(pixels[i + 1]);
		vec.push_back(pixels[i]);
		vec.push_back(pixels[i + 3]);
	}

	unsigned error = lodepng::encode(filename, vec, srcWidth, srcHeight);
	if (error) std::cout << lodepng_error_text(error) << std::endl;
}

HRESULT RayTracerApp::OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return OnRender();
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
