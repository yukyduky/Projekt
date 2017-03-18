#include <windows.h>
#include "WindowManager.h"
#include "DeferredRenderer.h"
#include "Input.h"
#include <crtdbg.h>

void EventLoop(MSG& msg, bool& run, Input& input);
void UpdateRender(InputVars& inVars, DeferredRenderer& dr);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WindowManager wm;
	DeferredRenderer dr;
	Input input;

	bool run = true;

	POINT p = { WIDTH / 2, HEIGHT / 2 };

	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	// Initializes the window
	if (!wm.InitializeWindow(hInstance, nCmdShow, WIDTH, HEIGHT, true))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Initializes DirectX
	if (!dr.InitializeDirectX(wm.getWinHandle()))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Initializes the Scene
	if (!dr.InitScene())
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	// Make cursor invisible
	ShowCursor(true);
	// Converts the window coordinates to screen coordinates
	ClientToScreen(wm.getWinHandle(), &p);

	SetCursorPos(p.x, p.y);

	// Game Loop!
	while (run)
	{
		// Handles window messages
		EventLoop(msg, run, input);

		// Update & Render
		UpdateRender(input.getInputVariables(), dr);

		// Resets the single keypress checker
		input.ResetWasKeyPressed();

		// Sets the cursor position every frame to make sure it doesnt leave the window
		SetCursorPos(p.x, p.y);
	}

	// Release the memory
	dr.Release();
	//DestroyWindow(wm.getWinHandle());

	return (int)msg.wParam;
}

void EventLoop(MSG& msg, bool& run, Input& input)
{
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			run = false;
			break;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			input.HandleKeyDown();
		}
		else if (msg.message == WM_KEYUP)
		{
			input.HandleKeyUp();
		}
		else if (msg.message == WM_MOUSEMOVE)
		{
			int xPos = LOWORD(msg.lParam);
			int yPos = HIWORD(msg.lParam);

			input.HandleMouse(xPos, yPos);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void UpdateRender(InputVars& inVars, DeferredRenderer& dr)
{
	dr.Update(inVars);
	dr.Render();
}
