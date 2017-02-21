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
	if (GetAsyncKeyState(VK_SPACE) != 0 && !keys[SPACE])
		keys[SPACE] = true;
	else if (GetAsyncKeyState(VK_SPACE) == 0 && keys[SPACE])
		keys[SPACE] = false;
	if (GetAsyncKeyState(VK_CONTROL) != 0 && !keys[CTRL])
		keys[CTRL] = true;
	else if (GetAsyncKeyState(VK_CONTROL) == 0 && keys[CTRL])
		keys[CTRL] = false;
	if (GetAsyncKeyState(VK_DOWN) != 0 && !keys[MB])
		keys[MB] = true;
	else if (GetAsyncKeyState(VK_DOWN) == 0 && keys[MB])
		keys[MB] = false;
	if (GetAsyncKeyState(VK_UP) != 0 && !keys[UP])
		keys[UP] = true;
	else if (GetAsyncKeyState(VK_UP) == 0 && keys[UP])
		keys[UP] = false;
	if (GetAsyncKeyState(VK_F) != 0 && !keys[F])
		keys[F] = true;
	else if (GetAsyncKeyState(VK_F) == 0 && keys[F])
		keys[F] = false;
}

void Input::ProcessMouse(Vector2 &mouseOffset)
{
	POINT p;

	// Gets the cursors current screen position
	GetCursorPos(&p);
	// Converts from screen to window coords
	ScreenToClient(GetActiveWindow(), &p);

	// Calculates the offset from the center of the window
	mouseOffset.x = (WIDTH / 2) - p.x;
	mouseOffset.y = (HEIGHT / 2) - p.y;
}
