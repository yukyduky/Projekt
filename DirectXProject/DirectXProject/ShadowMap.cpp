#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
}


ShadowMap::~ShadowMap()
{
}

bool ShadowMap::InitScene(ID3D11Device* gDevice)
{


	return true;
}

bool ShadowMap::Render(ID3D11DeviceContext* gDevCon)
{


	return true;
}

void ShadowMap::Release()
{
	gShadowMap->Release();
	gShadowSRV->Release();
	gShadowRTV->Release();
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

bool ShadowMap::MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void * cbPtr, int structSize)
{
	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	hr = gDevCon->Map(*gBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	if (hr != S_OK)
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

bool ShadowMap::BindTextureToRTVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexure, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV)
{
	// Describe the texture
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.CPUAccessFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create one texture per rendertarget
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		hr = gDevice->CreateTexture2D(&texDesc, nullptr, &gTexure[i]);
		if (hr != S_OK)
		{
			MessageBox(0, "Create shadow texture - Failed", "Error", MB_OK);
			return false;
		}
	}

	// Describe the Rendertargetview
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create one rtv per output from the pixel shader
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		hr = gDevice->CreateRenderTargetView(gTexure[i], &rtvDesc, &gRTV[i]);
		if (hr != S_OK)
		{
			MessageBox(0, "Create shadow RTV - Failed", "Error", MB_OK);
			return false;
		}
	}

	// Describe the Shaderresourceview
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create one srv per texture to be loaded into the next pixel shader
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		hr = gDevice->CreateShaderResourceView(gTexure[i], &srvDesc, &gSRV[i]);
		if (hr != S_OK)
		{
			MessageBox(0, "Create shadow SRV - Failed", "Error", MB_OK);
			return false;
		}
	}

	return true;
}
