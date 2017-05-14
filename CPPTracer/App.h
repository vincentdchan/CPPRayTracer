#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


class App
{
protected:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

public:
	App();
	~App();

	// Register the window class and call methods for instantiating drawing resources
	virtual HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

protected:
	// Initialize device-independent resources.
	virtual HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	virtual HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	virtual void DiscardDeviceResources();

	// Draw content.
	virtual HRESULT OnRender();

	// Use defined message
	virtual HRESULT OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return S_OK; }

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

private:

	void Render(int width, int height, int maxDepth);

};
