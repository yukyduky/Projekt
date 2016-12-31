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


bool GameManager::InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon)
{
	// Initialize the box
	if (!box.InitScene(gDevice, gDevCon))
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
}


bool GameManager::Render(ID3D11DeviceContext* gDevCon)
{
	box.Render(gDevCon);

	return true;
}


void GameManager::Release()
{
	box.Release();
}

Matrix GameManager::getMatrixWVP() const
{
	return wvp;
}

Matrix GameManager::getMatrixWorld() const
{
	return world;
}
