#include "GameManager.h"


GameManager::GameManager() // : pointLight(POINTLIGHT, Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 1.0f), 100.0f)
{
	world = XMMatrixIdentity();
	view = XMMatrixIdentity();
	proj = XMMatrixIdentity();

	mouseOffset.x = 0;
	mouseOffset.y = 0;

	time.StartTimer();

	dt = 0.0f;
	rotBoxY = 0.0f;
	rotBoxZ = 0.0f;
	transBox = 0.0f;

	for (int i = 0; i < NUM_KEYS; i++)
		keys[i] = false;

	pointLight.pos = Vector3(0.0f, 2.0f, -2.0f);
	pointLight.attenuation = Vector3(1.0f, 0.1f, 0.2f);
	pointLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.ambient = Vector3(0.1f, 0.1f, 0.1f);

	spotLight.pos = Vector3(0.0f, 0.0f, -3.0f);
	spotLight.dir = Vector3(0.0f, 0.0f, 1.0f);
	spotLight.angle = 20.0f;
	spotLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.ambient = Vector3(0.1f, 0.1f, 0.1f);
	spotLight.attenuation = Vector3(1.0f, 0.1f, 0.2f);

	directLight.pos = Vector3(0.0f, 2.0f, -2.0f);
	directLight.dir = Vector3(0.0f, 0.0f, 1.0f);
	directLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directLight.ambient = Vector3(0.1f, 0.1f, 0.1f);
}

GameManager::~GameManager()
{
}

bool GameManager::InitScene(ID3D11Device* gDevice)
{
	// Initialize the box
	if (!box.InitScene(gDevice))
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

	// Update the world matrices
	UpdateWorlds();

	// Update the Matrices
	wvp = boxWorld * view * proj;
	wvp = wvp.Transpose();
	world = boxWorld.Transpose();
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

PointLight GameManager::getPointLight() const
{
	return pointLight;
}

SpotLight GameManager::getSpotLight() const
{
	return spotLight;
}

DirectLight GameManager::getDirectLight() const
{
	return directLight;
}

void GameManager::UpdateWorlds()
{
	UpdateBox();
}

void GameManager::UpdateBox()
{
	Matrix rotate;
	Matrix translate;

	if (keys[SPACE] && keys[CTRL])
	{
		rotBoxZ += 0.5f * dt;
		if (rotBoxZ >= 6.28)
			rotBoxZ = 0;
	}
	else if (keys[CTRL])
	{
		transBox += 0.5f * dt;
	}
	else if (keys[SPACE])
	{
		rotBoxY += 0.5f * dt;
		if (rotBoxY >= 6.28)
			rotBoxY = 0;
	}
	rotate *= rotate.CreateRotationZ(rotBoxZ);
	rotate *= rotate.CreateRotationY(rotBoxY);
	//translate.Translation(Vector3(0.0f, 0.0f, transBox));


	boxWorld = rotate * translate;
}
