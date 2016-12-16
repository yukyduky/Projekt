#include "GameManager.h"


GameManager::GameManager()
{
	world = XMMatrixIdentity();
	view = XMMatrixIdentity();
	proj = XMMatrixIdentity();
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

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	// Create the SwapChain
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swapChainDesc, &gSwapChain, &gDevice, nullptr, &gDevCon);

	// Create the BackBuffer
	ID3D11Texture2D* BackBuffer;
	gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// Create the Render Target
	gDevice->CreateRenderTargetView(BackBuffer, nullptr, &gRenderTargetView);
	BackBuffer->Release();

	// Set the Render Target
	gDevCon->OMSetRenderTargets(1, &gRenderTargetView, nullptr);

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
	if (!shaderForward.CreateShaders(gDevice))
		return false;

	shaderForward.InitScene(gDevCon);

	if (!box.InitScene(gDevCon, gDevice))
		return false;
	
	return true;
}


void GameManager::Update()
{

}


void GameManager::Render()
{
	Color bgColor(255, 0, 255, 1.0f);
	// Clear the backbuffer
	gDevCon->ClearRenderTargetView(gRenderTargetView, bgColor);

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
	box.Release();
	shaderForward.Release();
}