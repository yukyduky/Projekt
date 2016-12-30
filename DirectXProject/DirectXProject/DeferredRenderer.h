#pragma once
#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "Shader.h"
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class DeferredRenderer
{
public:
	DeferredRenderer();
	~DeferredRenderer();

	bool InitializeDirectX(ID3D11Device* gDevice, IDXGISwapChain* gSwapChain);
	bool InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon);
	void Update(Matrix world, Matrix wvp);
	bool Render(ID3D11DeviceContext* gDevCon);
	void Release();

private:
	// Functions
	bool CreateShaders(ID3D11Device* gDevice);
	void SetGeoShaders(ID3D11DeviceContext* gDevCon);
	void SetLightShaders(ID3D11DeviceContext* gDevCon);
	bool CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize);
	bool CreateSampler(ID3D11Device* gDevice);
	bool MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr);
	//bool CreateSwapChain(HWND hwnd);
	bool CreateBackBufferRTV(ID3D11Device* gDevice, IDXGISwapChain* gSwapChain);
	bool CreateDepthStencilView(ID3D11Device* gDevice);
	void SetViewPort(ID3D11DeviceContext* gDevCon);

	// COMS
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11Texture2D* gDepthStencilBuffer;
	ID3D11Texture2D* gDiffuseMap;
	ID3D11Texture2D* gNormalMap;
	ID3D11SamplerState* gAnisoSampler;
	ID3D11RenderTargetView* gDeferredRTV[4];
	ID3D11RenderTargetView* gFinalRTV;
	ID3D11Buffer* gGeoObjBuffer;
	ID3D11Buffer* gGeoLightBuffer;
	ID3D11Buffer* gLightLightBuffer;

	// Objects
	Shader GeoShader;
	Shader LightShader;

	// Variables
	const wchar_t* fileNameGeoVertex = L"geoPassVert.hlsl";
	const wchar_t* fileNameGeoPixel = L"geoPassPixel.hlsl";
	const wchar_t* fileNameLightVertex = L"lightPassVert.hlsl";
	const wchar_t* fileNameLightPixel = L"lightPassPixel.hlsl";

	const D3D11_INPUT_ELEMENT_DESC geoInputDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D11_INPUT_ELEMENT_DESC lightInputDesc[1] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Structs
	struct Light
	{
		Vector4 pos;
		Vector3 color;
		float pad2;
		Vector3 dir;
		float pad3;
		Vector4 range;
		Vector2 spotlightAngles;
		Vector2 pad4;
	};

	// GeoShader Constant buffer: Object
	struct cbGeoObject
	{
		Matrix world;
		Matrix wvp;
	};
	cbGeoObject cbGeoObj;

	// GeoShader Constant buffer: Light
	struct cbGeoLighting
	{
		Vector3 specular;
		float specularPower;
		Matrix tangentW;
	};
	cbGeoLighting cbGeoLight;

	// LightShader Constant buffer: Light
	struct cbLightLighting
	{
		Light light;
		Vector3 cameraPos;
		float pad;
	};
	cbLightLighting cbLightLight;

	// Error handling
	HRESULT hr;
};

#endif
