#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

class Input
{
public:
	Input();
	~Input();

	void HandleInput(bool* keys, Vector2 &mouseOffset);

private:
	// Functions
	void ProcessKeyboard(bool* keys);
	void ProcessMouse(Vector2 &mouseOffset);
};

#endif
