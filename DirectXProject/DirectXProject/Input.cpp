#include "Input.h"



Input::Input()
{
}


Input::~Input()
{
}

void Input::HandleInput(bool* keys, Vector2 &mouseOffset)
{
	ProcessKeyboard(keys);
	ProcessMouse(mouseOffset);
}

void Input::ProcessKeyboard(bool* keys)
{
	if (GetAsyncKeyState(VK_W) != 0 && !keys[W])
		keys[W] = true;
	else if (GetAsyncKeyState(VK_W) == 0 && keys[W])
		keys[W] = false;
	if (GetAsyncKeyState(VK_A) != 0 && !keys[A])
		keys[A] = true;
	else if (GetAsyncKeyState(VK_A) == 0 && keys[A])
		keys[A] = false;
	if (GetAsyncKeyState(VK_S) != 0 && !keys[S])
		keys[S] = true;
	else if (GetAsyncKeyState(VK_S) == 0 && keys[S])
		keys[S] = false;
	if (GetAsyncKeyState(VK_D) != 0 && !keys[D])
		keys[D] = true;
	else if (GetAsyncKeyState(VK_D) == 0 && keys[D])
		keys[D] = false;
	if (GetAsyncKeyState(VK_ESCAPE) != 0 && !keys[ESC])
		keys[ESC] = true;
	else if (GetAsyncKeyState(VK_ESCAPE) == 0 && keys[ESC])
		keys[ESC] = false;
}

void Input::ProcessMouse(Vector2 &mouseOffset)
{
	POINT p;

	GetCursorPos(&p);
	ScreenToClient(GetActiveWindow(), &p);

	mouseOffset.x = (WIDTH / 2) - p.x;
	mouseOffset.y = (HEIGHT / 2) - p.y;
}