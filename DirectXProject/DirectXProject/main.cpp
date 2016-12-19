#include <windows.h>
#include "WindowManager.h"
#include "GameManager.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WindowManager wm;
	GameManager gm;

	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	// Initializes the window
	if (!wm.InitializeWindow(hInstance, nCmdShow, WIDTH, HEIGHT, true))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Initializes DirectX
	if (!gm.InitializeDirectX(hInstance, wm.getWinHandle()))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Initializes the Scene
	if (!gm.InitScene())
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Sets the viewport
	gm.SetViewport();

	// Game Loop!
	while (true)
	{
		// Handles window messages
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			// Update & Render
			gm.Update();
			gm.Render();
		}
	}

	// Release the memory
	gm.Release();
	//DestroyWindow(wm.getWinHandle());

	return (int)msg.wParam;
}