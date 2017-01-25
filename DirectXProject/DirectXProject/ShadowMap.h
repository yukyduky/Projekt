#pragma once
#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "Shader.h"
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct cbShadowLight
{
	Matrix world;
	Matrix view;
	Matrix proj;
	Matrix lightView;
	Matrix lightProj;
};

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool InitScene(ID3D11Device* gDevice);
	bool Render(ID3D11DeviceContext* gDevCon);
	void Release();

private:
	//Functions
	bool CreateShaders(ID3D11Device* gDevice);
	void SetShaders(ID3D11DeviceContext* gDevCon);
	bool MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr, int structSize);
	bool BindTextureToRTVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexure, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV);

	// COMS
	ID3D11Texture2D* gShadowMap;
	ID3D11ShaderResourceView* gShadowSRV;
	ID3D11RenderTargetView* gShadowRTV;

	// Objects
	Shader ShadowShader;

	// Variables
	const wchar_t* fileNameShadowVert = L"shadowPassVert.hlsl";
	const wchar_t* fileNameShadowPixel = L"shadowPassPixel.hlsl";
	UINT vertBufferStride;
	UINT vertBufferOffset;

	const D3D11_INPUT_ELEMENT_DESC shadowInpusDesc[SHADOW_INPUT_DESC_SIZE] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Error handling
	HRESULT hr;
};

#endif
