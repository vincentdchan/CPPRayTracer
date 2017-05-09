#include "App.h"
#include "RayTracerApp.h"

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		App * app = new RayTracerApp();

		if (SUCCEEDED(app->Initialize()))
		{
			app->RunMessageLoop();
		}

		delete app;
		CoUninitialize();
	}

	return 0;
}
