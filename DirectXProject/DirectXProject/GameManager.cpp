#include "GameManager.h"



GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}


bool GameManager::InitializeDirectX(HINSTANCE hInstance, HWND hwnd)
{
	//Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = WIDTH;
	bufferDesc.Height = HEIGHT;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &gDevice, NULL, &gDevCon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	gDevice->CreateRenderTargetView(BackBuffer, NULL, &gRenderTargetView);
	BackBuffer->Release();

	//Set our Render Target
	gDevCon->OMSetRenderTargets(1, &gRenderTargetView, NULL);

	return true;
}


bool GameManager::InitScene()
{
	return true;
}


void GameManager::Update()
{

}


void GameManager::Render()
{
	// Clear the backbuffer
	Color bgColor(255, 0, 255, 1.0f);

	gDevCon->ClearRenderTargetView(gRenderTargetView, bgColor);

	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}


void GameManager::Release()
{
	SwapChain->Release();
	gDevice->Release();
	gDevCon->Release();
}