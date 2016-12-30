#include "GameManager.h"


GameManager::GameManager()
{
	world = XMMatrixIdentity();
	view = XMMatrixIdentity();
	proj = XMMatrixIdentity();

	mouseOffset.x = 0;
	mouseOffset.y = 0;

	time.StartTimer();

	dt = 0.0f;

	for (int i = 0; i < NUM_KEYS; i++)
		keys[i] = false;
}


GameManager::~GameManager()
{
}


bool GameManager::InitializeDirectX(HWND hwnd)
{
	if (!CreateSwapChain(hwnd))
		return false;

	if (!dr.InitializeDirectX(gDevice, gSwapChain))
		return false;

	return true;
}


bool GameManager::InitScene()
{
	// Initialize the deferred renderer
	if (!dr.InitScene(gDevice, gDevCon))
		return false;

	// Create the shaders
	//if (!shaderForward.CreateShaders(gDevice))
	//	return false;

	// Initialize the shaders
	//shaderForward.InitScene(gDevCon);

	// Initialize the box
	if (!box.InitScene(gDevCon, gDevice))
		return false;
	
	// Get the matrices
	view = cam.getViewMatrix();
	proj = cam.getProjMatrix();

	return true;
}


void GameManager::Update()
{
	// Get time since last frame
	dt = float(time.GetFrameTime());

	// Handle keyboard & mouse
	input.HandleInput(keys, mouseOffset);

	// Update the camera
	cam.Update(keys, mouseOffset, dt);
	// Update the view matrix
	view = cam.getViewMatrix();

	// Update the box
	box.Update(dt);
	world = box.getWorldMatrix();

	// Update the Matrices
	wvp = world * view * proj;
	wvp = wvp.Transpose();
	world = world.Transpose();
	dr.Update(world, wvp);
}


bool GameManager::Render()
{
	if (!dr.Render(gDevCon))
		return false;

	// Present the backbuffer to the screen
	gSwapChain->Present(0, 0);

	return true;
}


void GameManager::Release()
{
	gDevice->Release();
	gDevCon->Release();
	gSwapChain->Release();

	box.Release();
	//shaderForward.Release();
	dr.Release();
}

bool GameManager::CreateSwapChain(HWND hwnd)
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
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swapChainDesc, &gSwapChain, &gDevice, nullptr, &gDevCon);
	if (hr != S_OK)
	{
		MessageBox(0, "Create Swapchain - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}
