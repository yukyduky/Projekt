#include "GameManager.h"


GameManager::GameManager()
{
	world = XMMatrixIdentity();
	view = XMMatrixIdentity();
	proj = XMMatrixIdentity();

	time.StartTimer();

	dt = 0.0f;
}


GameManager::~GameManager()
{
}


bool GameManager::InitializeDirectX(HINSTANCE hInstance, HWND hwnd)
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

	// Describe the SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	memset(&swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;                                    // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	swapChainDesc.OutputWindow = hwnd;                           // the window to be used
	swapChainDesc.SampleDesc.Count = 1;                               // how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;


	// Create the SwapChain
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swapChainDesc, &gSwapChain, &gDevice, nullptr, &gDevCon);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Swapchain - Failed", "Error", MB_OK);
		return false;
	}

	// Create the BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Swapchain backbuffer - Failed", "Error", MB_OK);
		return false;
	}

	// Create the Render Target
	hr = gDevice->CreateRenderTargetView(BackBuffer, nullptr, &gRenderTargetView);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Render Target View - Failed", "Error", MB_OK);
		return false;
	}
	BackBuffer->Release();

	CreateDepthStencilView();

	// Sets the Render Target
	gDevCon->OMSetRenderTargets(1, &gRenderTargetView, gDepthStencilView);
	return true;
}


void GameManager::SetViewport()
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

bool GameManager::InitScene()
{
	// Create the shaders
	if (!shaderForward.CreateShaders(gDevice))
		return false;

	// Initialize the shaders
	shaderForward.InitScene(gDevCon);

	// Initialize the box
	if (!box.InitScene(gDevCon, gDevice))
		return false;
	
	// Get the matrices
	view = cam.getViewMatrix();
	proj = cam.getProjMatrix();

	// Creates the buffer for the shaders
	if (!CreateConstBuffer())
		return false;

	return true;
}


void GameManager::Update()
{
	dt = time.GetFrameTime();

	box.Update(dt);

	world = box.getWorldMatrix();

	cbObj.wvp = world * view * proj;

	cbObj.wvp = cbObj.wvp.Transpose();
}


void GameManager::Render()
{
	Color bgColor(255, 0, 255, 1.0f);
	// Clear the backbuffer
	gDevCon->ClearRenderTargetView(gRenderTargetView, bgColor);
	// Clear the depth/Stencil view
	gDevCon->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDevCon->Map(gObjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// copy memory from CPU to GPU the entire struct
	memcpy(dataPtr.pData, &cbObj, sizeof(cbObject));
	// UnMap constant buffer so that we can use it again in the GPU
	gDevCon->Unmap(gObjBuffer, 0);

	//gDevCon->UpdateSubresource(gObjBuffer, 0, nullptr, &cbObj, 0, 0);
	// Set resource to Vertex Shader
	gDevCon->VSSetConstantBuffers(0, 1, &gObjBuffer);

	// Draws the box
	box.Render(gDevCon);
	// Present the backbuffer to the screen
	gSwapChain->Present(0, 0);
}


void GameManager::Release()
{
	gDevice->Release();
	gDevCon->Release();
	gSwapChain->Release();
	gRenderTargetView->Release();
	gObjBuffer->Release();
	gDepthStencilView->Release();
	gDepthStencilBuffer->Release();

	box.Release();
	shaderForward.Release();
}

bool GameManager::CreateConstBuffer()
{
	// Describes the constant buffer
	D3D11_BUFFER_DESC cbBufferDesc;
	memset(&cbBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	cbBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbBufferDesc.ByteWidth = sizeof(cbObject);
	cbBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBufferDesc.MiscFlags = 0;

	hr = gDevice->CreateBuffer(&cbBufferDesc, nullptr, &gObjBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Constant Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool GameManager::CreateDepthStencilView()
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

	return false;
}
