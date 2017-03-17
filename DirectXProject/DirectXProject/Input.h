#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

struct InputVars
{
	bool isKeyPressed[NUM_KEYS];
	bool wasKeyPressed[NUM_KEYS];
	Vector2 mouseOffset;
};

class Input
{
public:
	Input();
	~Input();

	void HandleKeyDown();
	void HandleKeyUp();
	void HandleMouse(int xPos, int yPos);
	void ResetWasKeyPressed();

	InputVars& getInputVariables();

private:
	// Variables
	InputVars inVars;
};

#endif
