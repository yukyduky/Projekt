#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"

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
	bool InitScene();
	// Update
	void Update();
	// Render
	void Render();
	// Release the memory
	void Release();

private:
	// COM Objects
	IDXGISwapChain* SwapChain;
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDevCon;
	ID3D11RenderTargetView* gRenderTargetView;
};

#endif
