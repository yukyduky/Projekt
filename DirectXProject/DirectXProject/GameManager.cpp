#include "GameManager.h"


GameManager::GameManager()
{
	staticWorld = XMMatrixIdentity();
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

	pointLight.pos = Vector3(0.0f, 10.0f, 0.0f);
	pointLight.attenuation = Vector3(1.0f, 0.05f, 0.0f);
	pointLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.ambient = Vector3(0.1f, 0.1f, 0.1f);
	pointLight.specPower = 50.0f;

	directLight.pos = Vector3(0.0f, 2.0f, -2.0f);
	directLight.dir = Vector3(0.0f, 0.0f, 1.0f);
	directLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directLight.ambient = Vector3(0.1f, 0.1f, 0.1f);
	directLight.specPower = 50.0f;

	gShadowSampler = spotLight.getSampler();
}

GameManager::~GameManager()
{
}

bool GameManager::InitScene(ID3D11Device* gDevice)
{
	// Create the gGeoObjBuffer for the shaders
	if (!CreateConstBuffer(gDevice, &gGeoObjBuffer, sizeof(cbGeoObject)))
		return false;

	// Create the gLightLightBuffer for the shaders
	if (!CreateConstBuffer(gDevice, &gLightLightBuffer, sizeof(cbLightLighting)))
		return false;

	// Initialize the box
	if (!box.InitScene(gDevice, Vector3(-1.0f, -1.0f, -1.0f), 2.0f))
		return false;

	if (!box2.InitScene(gDevice, Vector3(-0.5f, 10.0f, -0.5f), 8.0f))
		return false;

	if (!box3.InitScene(gDevice, Vector3(-3.5f, 1.0f, -0.5f), 3.0f))
		return false;

	if (!box4.InitScene(gDevice, Vector3(6.5f, 5.0f, 10.5f), 3.0f))
		return false;

	// Initialize the surface
	if (!surface.InitScene(gDevice))
		return false;
	
	// Initialize the spotlights
	if (!spotLight.InitScene(gDevice))
		return false;

	// Get the matrices
	view = cam.getViewMatrix();
	proj = cam.getProjMatrix();

	return true;
}

void GameManager::Update(ID3D11Device* gDevice)
{
	// Get time since last frame
	dt = float(time.GetFrameTime());

	// Handle keyboard & mouse
	input.HandleInput(keys, mouseOffset);

	// Update the camera
	cam.Update(keys, mouseOffset, dt, surface.heightValueList());
	// Update the view matrix
	view = cam.getViewMatrix();

	// Update the spotlights
	spotLight.Update();

	// Update the camera position for the general light attributes
	genLight.cameraPos = cam.getPosition();
	// Update Flashlight
	if(keys[CTRL])
		UpdateFlashLight(cam.getPosition(), cam.getForward());

	// Update the lights
	cbLightLight.pointLight = pointLight;
	cbLightLight.spotLight = spotLight.getSpotLight();
	cbLightLight.directLight = directLight;
	cbLightLight.genLight = genLight;

	// Update the world matrices
	UpdateWorlds();

	//Picking the boxes
	mouse.pickBoxes(keys[MB], *Boxes, cam, boxWorld, boxRotation, gDevice);

}

bool GameManager::Render(ID3D11DeviceContext* gDevCon)
{
	// Update the Matrices
	wvp = boxWorld * view * proj;
	
	cbGeoObj.wvp = wvp.Transpose();
	cbGeoObj.world = boxWorld.Transpose();

	if (!MapBuffer(gDevCon, &gGeoObjBuffer, &cbGeoObj, sizeof(cbGeoObj)))
		return false;
	// Set the constant buffer for the current vertex shader
	gDevCon->VSSetConstantBuffers(0, 1, &gGeoObjBuffer);

	// Render
	box.Render(gDevCon);
	box2.Render(gDevCon);
	box3.Render(gDevCon);
	box4.Render(gDevCon);

	// Update the Matrices
	wvp = staticWorld * view * proj;

	cbGeoObj.wvp = wvp.Transpose();
	cbGeoObj.world = staticWorld.Transpose();

	if (!MapBuffer(gDevCon, &gGeoObjBuffer, &cbGeoObj, sizeof(cbGeoObj)))
		return false;
	// Set the constant buffer for the current vertex shader
	gDevCon->VSSetConstantBuffers(0, 1, &gGeoObjBuffer);

	// Render
	surface.Render(gDevCon);

	// Map the LightLight constant buffer
	if (!MapBuffer(gDevCon, &gLightLightBuffer, &cbLightLight, sizeof(cbLightLight)))
		return false;
	// Set the constant buffer for the current pixel shader
	gDevCon->PSSetConstantBuffers(0, 1, &gLightLightBuffer);
	// Set the shadow sampler for the light pass
	gDevCon->PSSetSamplers(0, 1, &gShadowSampler);

	return true;
}

bool GameManager::CreateShadowMap(ID3D11DeviceContext* gDevCon, ID3D11ShaderResourceView** gSpotShadowMap)
{
	// Set the Rasterizerstate to allow depth bias
	gDevCon->RSSetState(spotLight.getRasterizer());
	// Update the world matrix
	spotLight.setWorld(boxWorld);
	// Update the spotlights
	spotLight.Update();

	if (!spotLight.Render(gDevCon))
		return false;

	// Render
	box.Render(gDevCon);
	box2.Render(gDevCon);

	// Update the world matrix for static objects
	spotLight.setWorld(staticWorld);
	// Update the spotlights
	spotLight.Update();
	spotLight.UpdateResources(gDevCon);

	// Render
	surface.Render(gDevCon);

	// Get the spotlight shadow maps
	*gSpotShadowMap = spotLight.getSRV();

	return true;
}

void GameManager::Release()
{
	gGeoObjBuffer->Release();
	gLightLightBuffer->Release();

	box.Release();
	box2.Release();
	surface.Release();
	spotLight.Release();
}

Matrix GameManager::getMatrixWVP() const
{
	return wvp;
}

Matrix GameManager::getMatrixWorld() const
{
	return staticWorld;
}

PointLight GameManager::getPointLight() const
{
	return pointLight;
}

bool GameManager::CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize)
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

bool GameManager::MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr, int structSize)
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
	translate.Translation(Vector3(0.0f, 0.0f, transBox));


	boxWorld = rotate * translate;
	boxRotation = rotate;

}

void GameManager::UpdateFlashLight(Vector3 position, Vector3 forward)
{
	SpotLightProperties tempSpotLight = spotLight.getSpotLight();
	
	tempSpotLight.dir = forward;
	tempSpotLight.pos = position;

	spotLight.setSpotLight(tempSpotLight);
}
