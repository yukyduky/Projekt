#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"
#include "Box.h"
#include "ForwardShader.h"
#include "Camera.h"
#include "Time.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class GameManager
{
public:
	GameManager();
	~GameManager();

	// Initializes DirectX
	bool InitializeDirectX(HINSTANCE hInstance, HWND hwnd);
	// Initializes the scene
	void SetViewport();
	// Initialize the scene
	bool InitScene();
	// Update
	void Update();
	// Render
	void Render();
	// Release the memory
	void Release();

private:
	// Functions
	bool CreateConstBuffer();
	bool CreateDepthStencilView();

	// COM Objects
	IDXGISwapChain* gSwapChain;
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDevCon;
	ID3D11RenderTargetView* gRenderTargetView;
	ID3D11Buffer* gObjBuffer;
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11Texture2D* gDepthStencilBuffer;

	// Matrices
	Matrix world;
	Matrix view;
	Matrix proj;

	// Objects
	Box box;
	ForwardShader shaderForward;
	Camera cam;
	Time time;

	// Variables
	double dt;

	// Constant buffer
	struct cbObject
	{
		Matrix wvp;
	};
	cbObject cbObj;

	// Error handling
	HRESULT hr;
};

#endif
