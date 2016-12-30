#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"
#include "Box.h"
//#include "ForwardShader.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "DeferredRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class GameManager
{
public:
	GameManager();
	~GameManager();

	// Initializes DirectX
	bool InitializeDirectX(HWND hwnd);
	// Initialize the scene
	bool InitScene();
	// Update
	void Update();
	// Render
	bool Render();
	// Release the memory
	void Release();

private:
	// Functions
	bool CreateSwapChain(HWND hwnd);

	// COMS
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDevCon;
	IDXGISwapChain* gSwapChain;

	// Matrices
	Matrix world;
	Matrix view;
	Matrix proj;
	Matrix wvp;
	
	// Vectors
	Vector2 mouseOffset;

	// Objects
	Box box;
	//ForwardShader shaderForward;
	Camera cam;
	Time time;
	Input input;
	DeferredRenderer dr;

	// Variables
	float dt;
	bool keys[NUM_KEYS];

	// Error handling
	HRESULT hr;
};

#endif
