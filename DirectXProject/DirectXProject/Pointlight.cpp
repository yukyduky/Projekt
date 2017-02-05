#include "Pointlight.h"



Pointlight::Pointlight()
{
}


Pointlight::~Pointlight()
{
}

bool Pointlight::BindTextureToDSVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexure, ID3D11DepthStencilView** gDSV, ID3D11ShaderResourceView** gSRV)
{
	// Describe the texture
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
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
