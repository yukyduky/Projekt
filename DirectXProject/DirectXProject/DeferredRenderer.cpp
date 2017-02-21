#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{
}

DeferredRenderer::~DeferredRenderer()
{
}

bool DeferredRenderer::InitializeDirectX(HWND hwnd)
{
	// Create Swapchain
	if (!CreateSwapChain(hwnd))
		return false;

	// Create the Rendertargetview with a backbuffer
	if (!CreateBackBufferRTV())
		return false;

	// Create the Depthstencilview
	if (!CreateDepthStencilView())
		return false;

	return true;
}

bool DeferredRenderer::InitScene()
{
	// Create the shaders
	if (!CreateShaders())
		return false;

	// Create the deferred RTV and bind the textures and SRVs to it
	if (!BindTextureToRTVAndSRV(gDeferredTex, gDeferredRTV, gDeferredSRV))
		return false;

	// Create the vertex buffer for the screen quad
	if (!CreateVertexBuffer())
		return false;

	// Create the sampler for the textures
	if (!CreateSampler())
		return false;

	if (!CreateRasterizerState())
		return false;

	if (!gm.InitScene(gDevice))
		return false;

	return true;
}

void DeferredRenderer::Update()
{
	gm.Update(gDevice);
}

bool DeferredRenderer::Render()
{
	// Prepare to run the GeoPass and store all the geometry data
	PreDrawing();

	// Draw the geometry
	gm.Render(gDevCon);

	// Prepare and draw the screen quad from the GeoPass data
	PostDrawing();

	return true;
}

void DeferredRenderer::Release()
{
	gDevice->Release();
	gDevCon->Release();
	gSwapChain->Release();
	gDepthStencilView->Release();
	gDepthStencilBuffer->Release();
	gAnisoSampler->Release();
	gVertBuffer->Release();
	gFinalRTV->Release();
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		gDeferredTex[i]->Release();
		gDeferredSRV[i]->Release();
		gDeferredRTV[i]->Release();
	}

	gm.Release();
}

bool DeferredRenderer::CreateShaders()
{
	// Create the GeoPass Shaders
	if (!GeoShader.CreateShaders(gDevice, fileNameGeoVertex, fileNameGeoPixel, geoInputDesc, GEO_INPUT_DESC_SIZE))
		return false;

	// Create the LightPass Shaders
	if (!LightShader.CreateShaders(gDevice, fileNameLightVertex, fileNameLightPixel, lightInputDesc, LIGHT_INPUT_DESC_SIZE))
		return false;

	return true;
}

void DeferredRenderer::SetGeoShaders()
{
	// Set the GeoShaders
	GeoShader.SetShaders(gDevCon);
}

void DeferredRenderer::SetLightShaders()
{
	// Set the LightShaders
	LightShader.SetShaders(gDevCon);
}

bool DeferredRenderer::CreateConstBuffer(ID3D11Buffer** gBuffer, int bufferSize)
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
		MessageBox(0, "Create Constant Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool DeferredRenderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rastDesc;
	memset(&rastDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0;
	rastDesc.SlopeScaledDepthBias = 0;

	hr = gDevice->CreateRasterizerState(&rastDesc, &gDefaultRasterizer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Default Rasterizer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool DeferredRenderer::CreateSampler()
{
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gDevice->CreateSamplerState(&sampDesc, &gAnisoSampler);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Samplerstate - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool DeferredRenderer::MapBuffer(ID3D11Buffer** gBuffer, void* cbPtr, int structSize)
{
	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	hr = gDevCon->Map(*gBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	if (FAILED(hr))
	{
		MessageBox(0, "Deferred Buffer mapping - Failed", "Error", MB_OK);
		return false;
	}
	// copy memory from CPU to GPU the entire struct
	memcpy(dataPtr.pData, cbPtr, structSize);
	// UnMap constant buffer so that we can use it again in the GPU
	gDevCon->Unmap(*gBuffer, 0);

	return true;
}

bool DeferredRenderer::CreateSwapChain(HWND hwnd)
{
	// Describe the SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;                                  // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // use 32-bit color
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how swap chain is to be used
	swapChainDesc.OutputWindow = hwnd;								// the window to be used
	swapChainDesc.SampleDesc.Count = 1;                             // how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Create the SwapChain
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, NULL, D3D11_SDK_VERSION, &swapChainDesc, &gSwapChain, &gDevice, nullptr, &gDevCon);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Swapchain - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool DeferredRenderer::CreateBackBufferRTV()
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
	if (FAILED(hr))
	{
		MessageBox(0, "Swapchain backbuffer - Failed", "Error", MB_OK);
		return false;
	}

	// Create the Render Target
	hr = gDevice->CreateRenderTargetView(BackBuffer, nullptr, &gFinalRTV);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Render Target View - Failed", "Error", MB_OK);
		return false;
	}
	BackBuffer->Release();

	return true;
}

bool DeferredRenderer::CreateDepthStencilView()
{
	// Describe the Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

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
	if (FAILED(hr))
	{
		MessageBox(0, "Create Depth Texture - Failed", "Error", MB_OK);
		return false;
	}

	hr = gDevice->CreateDepthStencilView(gDepthStencilBuffer, nullptr, &gDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Depth Stencil - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void DeferredRenderer::SetViewPort()
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

bool DeferredRenderer::BindTextureToRTVAndSRV(ID3D11Texture2D** gTexure, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV)
{
	// Describe the texture
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
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
		if (FAILED(hr))
		{
			MessageBox(0, "Create deferred texture - Failed", "Error", MB_OK);
			return false;
		}
	}

	// Describe the Rendertargetview
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create one rtv per output from the pixel shader
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		hr = gDevice->CreateRenderTargetView(gTexure[i], &rtvDesc, &gRTV[i]);
		if (FAILED(hr))
		{
			MessageBox(0, "Create deferred RTV - Failed", "Error", MB_OK);
			return false;
		}
	}

	// Describe the Shaderresourceview
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create one srv per texture to be loaded into the next pixel shader
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		hr = gDevice->CreateShaderResourceView(gTexure[i], &srvDesc, &gSRV[i]);
		if (FAILED(hr))
		{
			MessageBox(0, "Create deferred SRV - Failed", "Error", MB_OK);
			return false;
		}
	}

	return true;
}

bool DeferredRenderer::CreateVertexBuffer()
{
	struct Vertex
	{
		float x, y, z, w;
	};

	Vertex v[] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
	};

	// Describe the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Set the vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexData;
	memset(&vertexData, 0, sizeof(vertexData));
	vertexData.pSysMem = &v;

	hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &gVertBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Vertex buffer - Failed", "Error", MB_OK);
		return false;
	}

	vertBufferStride = sizeof(Vertex);
	vertBufferOffset = 0;

	return true;
}

bool DeferredRenderer::PreDrawing()
{
	// Create the shadowmaps
	gm.CreateShadowMap(gDevCon, &gSpotShadowMap);

	// Set the Rasterizerstate to the default state
	gDevCon->RSSetState(gDefaultRasterizer);

	Color bgColor(255, 0, 255, 1.0f);
	// Clear the backbuffer
	gDevCon->ClearRenderTargetView(gFinalRTV, bgColor);
	// Clear all the deferred RTVs
	for (int i = 0; i < NUM_DEFERRED_OUTPUTS; i++)
	{
		gDevCon->ClearRenderTargetView(gDeferredRTV[i], bgColor);
	}
	// Clear the DSV
	gDevCon->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the GeoShaders to the current shaders
	SetGeoShaders();
	// Set the viewport
	SetViewPort();
	// Set the render target to the deferred RTV 
	gDevCon->OMSetRenderTargets(NUM_DEFERRED_OUTPUTS, gDeferredRTV, gDepthStencilView);
	// Set the texture sampler for the current pixel shader
	gDevCon->PSSetSamplers(0, 1, &gAnisoSampler);

	return true;
}

bool DeferredRenderer::PostDrawing()
{
	// Set the LightShaders to the current shaders
	SetLightShaders();
	// Set the vertexbuffer for the screen quad
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &vertBufferStride, &vertBufferOffset);
	// Set Primitive Topology
	gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// Set the Render Target to the final RTV
	gDevCon->OMSetRenderTargets(1, &gFinalRTV, gDepthStencilView);

	// Set the normal texture for the current pixel shader
	gDevCon->PSSetShaderResources(0, NUM_DEFERRED_OUTPUTS, gDeferredSRV);
	gDevCon->PSSetShaderResources(4, 1, &gSpotShadowMap);

	// Draw the final texture over the whole screen
	gDevCon->Draw(4, 0);

	// Present the backbuffer to the screen
	gSwapChain->Present(0, 0);

	return true;
}
