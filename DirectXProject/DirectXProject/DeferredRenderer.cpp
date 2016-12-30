#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{

}


DeferredRenderer::~DeferredRenderer()
{
}

bool DeferredRenderer::InitializeDirectX(ID3D11Device* gDevice, IDXGISwapChain* gSwapChain)
{
	// Create the Rendertargetview with a backbuffer
	if (!CreateBackBufferRTV(gDevice, gSwapChain))
		return false;

	// Create the Depthstencilview
	if (!CreateDepthStencilView(gDevice))
		return false;

	return true;
}

bool DeferredRenderer::InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon)
{
	// Set the viewport
	SetViewPort(gDevCon);

	// Create the shaders
	if (!CreateShaders(gDevice))
		return false;

	// Create the gGeoObjBuffer for the shaders
	if (!CreateConstBuffer(gDevice, &gGeoObjBuffer, sizeof(cbGeoObject)))
		return false;

	// Create the gGeoLightBuffer for the shaders
	if (!CreateConstBuffer(gDevice, &gGeoLightBuffer, sizeof(cbGeoLighting)))
		return false;

	// Create the gLightLightBuffer for the shaders
	if (!CreateConstBuffer(gDevice, &gLightLightBuffer, sizeof(cbLightLighting)))
		return false;

	// Create the sampler for the textures
	if (!CreateSampler(gDevice))
		return false;

	return true;
}

void DeferredRenderer::Update(Matrix world, Matrix wvp)
{
	// Update the matrices
	cbGeoObj.world = world;
	cbGeoObj.wvp = wvp;
}

bool DeferredRenderer::Render(ID3D11DeviceContext* gDevCon)
{
	Color bgColor(255, 0, 255, 1.0f);
	// Clear the backbuffer
	gDevCon->ClearRenderTargetView(gFinalRTV, bgColor);

	// Generic pointer
	void* cbPtr = nullptr;

	// Set the GeoShaders to the current shaders
	SetGeoShaders(gDevCon);
	// Set the render target to the deferred RTV
	gDevCon->OMSetRenderTargets(4, gDeferredRTV, gDepthStencilView);

	// Draw?

	// Map the GeoObject constant buffer
	cbPtr = &cbGeoObj;
	if (!MapBuffer(gDevCon, &gGeoObjBuffer, cbPtr))
		return false;
	// Set the constant buffer for the current vertex shader
	gDevCon->VSSetConstantBuffers(0, 1, &gGeoObjBuffer);
	// Map the GeoLight constant buffer
	cbPtr = &cbGeoLight;
	if (!MapBuffer(gDevCon, &gGeoLightBuffer, cbPtr))
		return false;
	// Set the constant buffer for the current pixel shader
	gDevCon->PSSetConstantBuffers(0, 1, &gGeoLightBuffer);

	// Set the LightShaders to the current shaders
	SetLightShaders(gDevCon);
	// Set the Render Target to the final RTV
	gDevCon->OMSetRenderTargets(1, &gFinalRTV, gDepthStencilView);


	// Map the LightLight constant buffer
	cbPtr = &cbLightLight;
	if (!MapBuffer(gDevCon, &gLightLightBuffer, cbPtr))
		return false;
	// Set the constant buffer for the current pixel shader
	gDevCon->PSSetConstantBuffers(0, 1, &gLightLightBuffer);

	return true;
}

void DeferredRenderer::Release()
{
	gDepthStencilView->Release();
	gDepthStencilBuffer->Release();
	//gDiffuseMap->Release();
	//gNormalMap->Release();
	gAnisoSampler->Release();
	gGeoObjBuffer->Release();
	gGeoLightBuffer->Release();
	gLightLightBuffer->Release();
}

bool DeferredRenderer::CreateShaders(ID3D11Device* gDevice)
{
	// Create the GeoPass Shaders
	if (!GeoShader.CreateShaders(gDevice, fileNameGeoVertex, fileNameGeoPixel, geoInputDesc, GEO_INPUT_DESC_SIZE))
		return false;

	// Create the LightPass Shaders
	if (!LightShader.CreateShaders(gDevice, fileNameLightVertex, fileNameLightPixel, lightInputDesc, LIGHT_INPUT_DESC_SIZE))
		return false;

	return true;
}

void DeferredRenderer::SetGeoShaders(ID3D11DeviceContext* gDevCon)
{
	// Set the GeoShaders
	GeoShader.SetShaders(gDevCon);
}

void DeferredRenderer::SetLightShaders(ID3D11DeviceContext* gDevCon)
{
	// Set the LightShaders
	LightShader.SetShaders(gDevCon);
}

bool DeferredRenderer::CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize)
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
	if (hr != S_OK)
	{
		MessageBox(0, "Create Constant Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool DeferredRenderer::CreateSampler(ID3D11Device* gDevice)
{
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gDevice->CreateSamplerState(&sampDesc, &gAnisoSampler);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Samplerstate - Failed", "Error", MB_OK);
		return false;
	}
}

bool DeferredRenderer::MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr)
{
	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	hr = gDevCon->Map(*gBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	if (hr != S_OK)
	{
		MessageBox(0, "Buffer mapping - Failed", "Error", MB_OK);
		return false;
	}
	// copy memory from CPU to GPU the entire struct
	memcpy(dataPtr.pData, &cbPtr, sizeof(&cbPtr));
	// UnMap constant buffer so that we can use it again in the GPU
	gDevCon->Unmap(*gBuffer, 0);

	return true;
}

bool DeferredRenderer::CreateBackBufferRTV(ID3D11Device* gDevice, IDXGISwapChain* gSwapChain)
{
	// Describe the Buffer
	DXGI_MODE_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = WIDTH;
	bufferDesc.Height = HEIGHT;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Create the BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Swapchain backbuffer - Failed", "Error", MB_OK);
		return false;
	}

	// Create the Render Target
	hr = gDevice->CreateRenderTargetView(BackBuffer, nullptr, &gFinalRTV);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Render Target View - Failed", "Error", MB_OK);
		return false;
	}
	BackBuffer->Release();

	return true;
}

bool DeferredRenderer::CreateDepthStencilView(ID3D11Device* gDevice)
{
	// Describe the Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

	depthStencilDesc.Width = WIDTH;
	depthStencilDesc.Height = HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Creates the Depth/Stencil View
	hr = gDevice->CreateTexture2D(&depthStencilDesc, nullptr, &gDepthStencilBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Depth Texture - Failed", "Error", MB_OK);
		return false;
	}

	hr = gDevice->CreateDepthStencilView(gDepthStencilBuffer, nullptr, &gDepthStencilView);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Depth Stencil - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void DeferredRenderer::SetViewPort(ID3D11DeviceContext* gDevCon)
{
	D3D11_VIEWPORT viewport;
	memset(&viewport, 0, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = WIDTH;
	viewport.Height = HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set the Viewport
	gDevCon->RSSetViewports(1, &viewport);
}
