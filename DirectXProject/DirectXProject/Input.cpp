#include "Input.h"


Input::Input()
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		inVars.isKeyPressed[i] = false;
		inVars.wasKeyPressed[i] = false;
	}
}

Input::~Input()
{
}

void Input::HandleKeyDown()
{
	if (GetAsyncKeyState(VK_W) != 0 && !inVars.isKeyPressed[W])
	{
		inVars.wasKeyPressed[W] = true;
		inVars.isKeyPressed[W] = true;
	}
	else if (GetAsyncKeyState(VK_A) != 0 && !inVars.isKeyPressed[A])
	{
		inVars.wasKeyPressed[A] = true;
		inVars.isKeyPressed[A] = true;
	}
	else if (GetAsyncKeyState(VK_S) != 0 && !inVars.isKeyPressed[S])
	{
		inVars.wasKeyPressed[S] = true;
		inVars.isKeyPressed[S] = true;
	}
	else if (GetAsyncKeyState(VK_D) != 0 && !inVars.isKeyPressed[D])
	{
		inVars.wasKeyPressed[D] = true;
		inVars.isKeyPressed[D] = true;
	}
	else if (GetAsyncKeyState(VK_ESCAPE) != 0 && !inVars.isKeyPressed[ESC])
	{
		inVars.wasKeyPressed[ESC] = true;
		inVars.isKeyPressed[ESC] = true;
	}
	else if (GetAsyncKeyState(VK_SPACE) != 0 && !inVars.isKeyPressed[SPACE])
	{
		inVars.wasKeyPressed[SPACE] = true;
		inVars.isKeyPressed[SPACE] = true;
	}
	else if (GetAsyncKeyState(VK_CONTROL) != 0 && !inVars.isKeyPressed[CTRL])
	{
		inVars.wasKeyPressed[CTRL] = true;
		inVars.isKeyPressed[CTRL] = true;
	}
	else if (GetAsyncKeyState(VK_DOWN) != 0 && !inVars.isKeyPressed[MB])
	{
		inVars.wasKeyPressed[MB] = true;
		inVars.isKeyPressed[MB] = true;
	}
	else if (GetAsyncKeyState(VK_UP) != 0 && !inVars.isKeyPressed[UP])
	{
		inVars.isKeyPressed[UP] = true;
		inVars.wasKeyPressed[UP] = true;
	}
	else if (GetAsyncKeyState(VK_F) != 0 && !inVars.isKeyPressed[F])
	{
		inVars.wasKeyPressed[F] = true;
		inVars.isKeyPressed[F] = true;
	}
}

void Input::HandleKeyUp()
{
	if (GetAsyncKeyState(VK_W) == 0 && inVars.isKeyPressed[W])
		inVars.isKeyPressed[W] = false;
	else if (GetAsyncKeyState(VK_A) == 0 && inVars.isKeyPressed[A])
		inVars.isKeyPressed[A] = false;
	else if (GetAsyncKeyState(VK_S) == 0 && inVars.isKeyPressed[S])
		inVars.isKeyPressed[S] = false;
	else if (GetAsyncKeyState(VK_D) == 0 && inVars.isKeyPressed[D])
		inVars.isKeyPressed[D] = false;
	else if (GetAsyncKeyState(VK_ESCAPE) == 0 && inVars.isKeyPressed[ESC])
		inVars.isKeyPressed[ESC] = false;
	else if (GetAsyncKeyState(VK_SPACE) == 0 && inVars.isKeyPressed[SPACE])
		inVars.isKeyPressed[SPACE] = false;
	else if (GetAsyncKeyState(VK_CONTROL) == 0 && inVars.isKeyPressed[CTRL])
		inVars.isKeyPressed[CTRL] = false;
	else if (GetAsyncKeyState(VK_DOWN) == 0 && inVars.isKeyPressed[MB])
		inVars.isKeyPressed[MB] = false;
	else if (GetAsyncKeyState(VK_UP) == 0 && inVars.isKeyPressed[UP])
		inVars.isKeyPressed[UP] = false;
	else if (GetAsyncKeyState(VK_F) == 0 && inVars.isKeyPressed[F])
		inVars.isKeyPressed[F] = false;
}

void Input::HandleMouse(int xPos, int yPos)
{
	inVars.mouseOffset.x = WIDTH / 2 - xPos;
	inVars.mouseOffset.y = HEIGHT / 2 - yPos;
}

void Input::ResetWasKeyPressed()
{
	for (int i = 0; i < NUM_KEYS; i++)
		inVars.wasKeyPressed[i] = false;
}

InputVars& Input::getInputVariables()
{
	return inVars;
}
