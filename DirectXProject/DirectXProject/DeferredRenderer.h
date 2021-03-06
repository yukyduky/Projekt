#pragma once
#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "Input.h"
#include "GameManager.h"
#include "Shader.h"
#include "globals.h"
#include "Blur.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class DeferredRenderer
{
public:
	DeferredRenderer();
	~DeferredRenderer();

	bool InitializeDirectX(HWND hwnd);
	bool InitScene();
	void Update(InputVars inVars);
	bool Render();
	void Release();

private:
	// Functions
	bool CreateShaders();
	void SetGeoShaders();
	void SetLightShaders();
	bool CreateRasterizerState();
	bool CreateRasterizerState(bool wireframe);
	bool CreateSampler();
	bool CreateSwapChain(HWND hwnd);
	bool CreateBackBufferRTV();
	bool CreateDepthStencilView();
	void SetViewPort();
	bool BindTextureToRTVAndSRV(ID3D11Texture2D** gTexure, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV, const int nrOfOutputs);
	bool CreateVertexBuffer();
	bool PreDrawing();
	bool PostDrawing();

	// COMS
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDevCon;
	IDXGISwapChain* gSwapChain;
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11Texture2D* gDepthStencilBuffer;
	ID3D11Texture2D* gDeferredTex[NUM_DEFERRED_OUTPUTS];
	ID3D11ShaderResourceView* gDeferredSRV[NUM_DEFERRED_OUTPUTS];
	ID3D11RenderTargetView* gDeferredRTV[NUM_DEFERRED_OUTPUTS];
	ID3D11RenderTargetView* gFinalRTV;
	ID3D11SamplerState* gAnisoSampler;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gSpotShadowMap;
	ID3D11RasterizerState* gDefaultRasterizer;
	ID3D11RasterizerState* gWireframeRasterizer;

	// Objects
	GameManager gm;
	Blur Bluring;	
	Shader GeoShader;
	Shader LightShader;

	// Variables
	const wchar_t* fileNameGeoVertex = L"geoPassVert.hlsl";
	const wchar_t* fileNameGeoPixel = L"geoPassPixel.hlsl";
	const wchar_t* fileNameGeoHull = L"geoPassHull.hlsl";
	const wchar_t* fileNameGeoDomain = L"geoPassDomain.hlsl";
	const wchar_t* fileNameGeoGeometry = L"geoPassGeometry.hlsl";
	const wchar_t* fileNameLightVertex = L"lightPassVert.hlsl";
	const wchar_t* fileNameLightPixel = L"lightPassPixel.hlsl";
	UINT vertBufferStride;
	UINT vertBufferOffset;
	bool changeFillMode = false;
	bool wireframe = false;

	const D3D11_INPUT_ELEMENT_DESC geoInputDesc[GEO_INPUT_DESC_SIZE] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D11_INPUT_ELEMENT_DESC lightInputDesc[LIGHT_INPUT_DESC_SIZE] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Error handling
	HRESULT hr;
};

#endif
