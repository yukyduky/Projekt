#pragma once
#ifndef _BLUR_H_
#define _BLUR_H_

#include <d3d11.h>
#include <SimpleMath.h>
#include "Shader.h"
#include "globals.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


class Blur
{
public:
	Blur();
	~Blur();

	void Release();
	bool InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon);
	bool CreateShaders(ID3D11Device* gDevice);
	bool BindTextures(ID3D11Device* gDevice);
	void SetDiffuseGlowPassShaders(ID3D11DeviceContext* gDevCon);
	void SetHorizontalBlurShaders(ID3D11DeviceContext* gDevCon);
	void SetVerticalBlurShaders(ID3D11DeviceContext* gDevCon);
	bool CreateVertexBuffer(ID3D11Device* gDevice);
	void SetVertexBuffer(ID3D11DeviceContext* gDevCon);
	bool CreateSamplerState(ID3D11Device* gDevice);
	void SetSamplerState(ID3D11DeviceContext* gDevCon);
	bool CreateConstantBuffers(ID3D11Device* gDevice);
	bool BindTextureToRTVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexture, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV);


	void SetHorizontalBlurRTV(ID3D11DeviceContext* gDevCon);
	void SetHorizontalBlurSRV(ID3D11DeviceContext* gDevCon);

	void SetVerticalBlurRTV(ID3D11DeviceContext* gDevCon);
	void SetVerticalBlurSRV(ID3D11DeviceContext* gDevCon);

	void SetDiffuseGlowRTV(ID3D11DeviceContext* gDevCon);
	void SetDiffuseGlowSRV(ID3D11DeviceContext* gDevCon);

	bool CreateConstantBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize);
	bool MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr, int structSize);

private:

	struct cbObject
	{
		float screenWidth;
		float screenHeight;
		float padding1;
		float padding2;
	};

	//COMS
	ID3D11Buffer* gVertBuffer;
	ID3D11SamplerState* blurSamplerState;
	ID3D11Buffer* cbObjBuffer;

	ID3D11Texture2D* gHorizontalBlurTex;
	ID3D11ShaderResourceView* gHorizontalBlurSRV;
	ID3D11RenderTargetView* gHorizontalBlurRTV;

	ID3D11Texture2D* gVerticalBlurTex;
	ID3D11ShaderResourceView* gVerticalBlurSRV;
	ID3D11RenderTargetView* gVerticalBlurRTV;
	
	ID3D11Texture2D* gDiffuseGlowTex;
	ID3D11ShaderResourceView* gDiffuseGlowSRV;
	ID3D11RenderTargetView* gDiffuseGlowRTV;



	//Shaders for horizontal and vertical blur.
	Shader diffuseGlowShader;
	Shader horizontalBlurShader;
	Shader verticalBlurShader;
	Shader diffuseBlurShader;

	//Constant buffers
	cbObject cbObj;

	//Variables
	HRESULT hr;
	const wchar_t* fileNameHorizontalBlurVertex = L"horizontalBlurVertex.hlsl";
	const wchar_t* fileNameHorizontalBlurPixel = L"horizontalBlurPixel.hlsl";
	const wchar_t* fileNameVerticalBlurVertex = L"verticalBlurVertex.hlsl";
	const wchar_t* fileNameVerticalBlurPixel = L"verticalBlurPixel.hlsl";

	const wchar_t* fileNameDiffuseGlowPassVertex = L"diffuseGlowPassVertex.hlsl";
	const wchar_t* fileNameDiffuseGlowPassPixel = L"diffuseGlowPassPixel.hlsl";
	const wchar_t* fileNameDiffuseBlurPassVertex = L"diffuseBlurPassVertex.hlsl";
	const wchar_t* fileNameDiffuseBlurPassPixel = L"diffuseBlurPassPixel.hlsl";

	UINT vertBufferStride;
	UINT vertBufferOffset;

	const D3D11_INPUT_ELEMENT_DESC inputDesc[BLUR_INPUT_DESC_SIZE] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

};

#endif