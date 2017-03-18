#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

bool ShadowMap::InitScene(ID3D11Device* gDevice)
{
	// Create the shaders
	if (!CreateShaders(gDevice))
		return false;

	if (!CreateRasterizerState(gDevice))
		return false;

	CreateViewPort();

	return true;
}

void ShadowMap::Release()
{
	gShadowSampler->Release();
	gDepthBiasRasterizer->Release();
	ShadowShader.Release();
}

ID3D11SamplerState* ShadowMap::getSampler() const
{
	return gShadowSampler;
}

ID3D11RasterizerState * ShadowMap::getRasterizer() const
{
	return gDepthBiasRasterizer;
}

void ShadowMap::setWorld(Matrix world)
{
	this->world = world;
}

Matrix ShadowMap::getWorld() const
{
	return world;
}

bool ShadowMap::CreateShaders(ID3D11Device* gDevice)
{
	if (!ShadowShader.CreateShaders(gDevice, fileNameShadowVert, fileNameShadowPixel, shadowInpusDesc, SHADOW_INPUT_DESC_SIZE))
		return false;

	return true;
}

void ShadowMap::SetShaders(ID3D11DeviceContext* gDevCon)
{
	ShadowShader.SetShaders(gDevCon);
}

bool ShadowMap::CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize)
{
	// Describes the constant buffer
	D3D11_BUFFER_DESC cbBufferDesc;
	memset(&cbBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	cbBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbBufferDesc.ByteWidth = bufferSize;
	cbBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBufferDesc.MiscFlags = 0;

	hr = gDevice->CreateBuffer(&cbBufferDesc, nullptr, gBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create shadow Constant Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool ShadowMap::CreateSampler(ID3D11Device* gDevice)
{
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gDevice->CreateSamplerState(&sampDesc, &gShadowSampler);
	if (FAILED(hr))
	{
		MessageBox(0, "Create shadow Samplerstate - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool ShadowMap::MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void * cbPtr, int structSize)
{
	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	hr = gDevCon->Map(*gBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	if (FAILED(hr))
	{
		MessageBox(0, "Shadow Buffer mapping - Failed", "Error", MB_OK);
		return false;
	}
	// copy memory from CPU to GPU the entire struct
	memcpy(dataPtr.pData, cbPtr, structSize);
	// UnMap constant buffer so that we can use it again in the GPU
	gDevCon->Unmap(*gBuffer, 0);

	return true;
}

bool ShadowMap::BindTextureToDSVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexure, ID3D11DepthStencilView** gDSV, ID3D11ShaderResourceView** gSRV, int vpWidth, int vpHeight)
{
	// Describe the texture
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = vpWidth;
	texDesc.Height = vpHeight;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create the texture to draw the shadowmap on
	hr = gDevice->CreateTexture2D(&texDesc, nullptr, gTexure);
	if (FAILED(hr))
	{
		MessageBox(0, "Create shadow texture - Failed", "Error", MB_OK);
		return false;
	}

	// Describe the depthstencilview
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	memset(&dsvDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = gDevice->CreateDepthStencilView(*gTexure, &dsvDesc, gDSV);
	if (FAILED(hr))
	{
		MessageBox(0, "Create shadow DSV - Failed", "Error", MB_OK);
		return false;
	}

	// Describe the Shaderresourceview
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the SRV for the shadowmap
	hr = gDevice->CreateShaderResourceView(*gTexure, &srvDesc, gSRV);
	if (FAILED(hr))
	{
		MessageBox(0, "Create shadow SRV - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool ShadowMap::CreateRasterizerState(ID3D11Device* gDevice)
{
	D3D11_RASTERIZER_DESC rastDesc;
	memset(&rastDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthBias = 1000;
	rastDesc.DepthBiasClamp = 0.01f;
	rastDesc.SlopeScaledDepthBias = 0.0f;

	hr = gDevice->CreateRasterizerState(&rastDesc, &gDepthBiasRasterizer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Depthbias Rasterizer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void ShadowMap::SetViewPort(ID3D11DeviceContext* gDevCon)
{
	// Set the Viewport
	gDevCon->RSSetViewports(1, &viewport);
}

void ShadowMap::CreateViewPort()
{
	memset(&viewport, 0, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = vpWidth;
	viewport.Height = vpHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}
