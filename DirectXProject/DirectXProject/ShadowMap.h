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

	void Release();

	ID3D11SamplerState* getSampler() const;
	ID3D11RasterizerState* getRasterizer() const;

	void setWorld(Matrix world);
	Matrix getWorld() const;

protected:
	//Functions
	bool InitScene(ID3D11Device* gDevice);
	bool CreateShaders(ID3D11Device* gDevice);
	void SetShaders(ID3D11DeviceContext* gDevCon);
	bool CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize);
	bool CreateSampler(ID3D11Device* gDevice);
	bool MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr, int structSize);
	bool BindTextureToDSVAndSRV(ID3D11Device * gDevice, ID3D11Texture2D ** gTexure, ID3D11DepthStencilView ** gDSV, ID3D11ShaderResourceView ** gSRV, int vpWidth, int vpHeight);
	bool CreateRasterizerState(ID3D11Device* gDevice);
	void SetViewPort(ID3D11DeviceContext* gDevCon);
	void CreateViewPort();

	// Matrices
	Matrix wvp;
	Matrix world;
	Matrix view;
	Matrix proj;

	// Variables
	int vpWidth;
	int vpHeight;

	// Structs
	struct cbShadowObject
	{
		Matrix wvp;
	};

	// Objects
	cbShadowObject cbShadowObj;

private:
	// COMS
	ID3D11SamplerState* gShadowSampler;
	ID3D11RasterizerState* gDepthBiasRasterizer;

	// D3Ds
	D3D11_VIEWPORT viewport;

	// Objects
	Shader ShadowShader;

	// Variables
	const wchar_t* fileNameShadowVert = L"shadowPassVert.hlsl";
	const wchar_t* fileNameShadowPixel = L"shadowPassPixel.hlsl";

	const D3D11_INPUT_ELEMENT_DESC shadowInpusDesc[SHADOW_INPUT_DESC_SIZE] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Error handling
	HRESULT hr;
};

#endif
