#include "Spotlight.h"



Spotlight::Spotlight()
{
	spotLight.pos = Vector3(0.0f, 8.0f, 0.0f);
	spotLight.dir = Vector3(0.0f, -1.0f, 0.0f);
	spotLight.angle = 10.0f;
	spotLight.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.ambient = Vector3(0.1f, 0.1f, 0.1f);
	spotLight.attenuation = Vector3(1.0f, 0.0000005f, 0.0f);
	spotLight.specPower = 50.0f;

	right = Vector3(1.0f, 0.0f, 0.0f);
	up = XMVector3Normalize(XMVector3Cross(spotLight.dir, right));

	proj = XMMatrixPerspectiveFovLH(0.40f * XM_PI, (float)WIDTH / HEIGHT, 0.1f, 1000.0f);

	vpWidth = 800;
	vpHeight = 600;
}


Spotlight::~Spotlight()
{
}

bool Spotlight::InitScene(ID3D11Device * gDevice)
{
	// Initialize the Shadowmap class
	if (!ShadowMap::InitScene(gDevice))
		return false;

	if (!CreateConstBuffer(gDevice, &gShadowObjBuffer, sizeof(cbShadowObject)))
		return false;

	// Link the DSV and the SRV to the texture
	if (!BindTextureToDSVAndSRV(gDevice, &gShadowMap, &gShadowDSV, &gShadowSRV, vpWidth, vpHeight))
		return false;

	// Create the sampler for the shadows
	if (!CreateSampler(gDevice))
		return false;

	return true;
}

void Spotlight::Update()
{
	// Update the view matrix
	view = XMMatrixLookAtLH(spotLight.pos, spotLight.pos + spotLight.dir, up);

	// Update the shadow matrix aka the light projection
	shadowMatrix = view * proj;
	spotLight.shadowMatrix = shadowMatrix.Transpose();

	// Update the wvp for the light
	wvp = world * view * proj;
	cbShadowObj.wvp = wvp.Transpose();
}

bool Spotlight::Render(ID3D11DeviceContext* gDevCon)
{
	// Clear the DSV
	gDevCon->ClearDepthStencilView(gShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// Set the ShadowShaders to the current shaders
	SetShaders(gDevCon);
	// Set the viewport
	SetViewPort(gDevCon);
	// Set the RTV to NULL as we just want to render the depthbuffer
	gDevCon->OMSetRenderTargets(0, nullptr, gShadowDSV);

	if (!UpdateResources(gDevCon))
		return false;

	return true;
}

bool Spotlight::UpdateResources(ID3D11DeviceContext* gDevCon)
{
	// Map the constant buffer
	if (!MapBuffer(gDevCon, &gShadowObjBuffer, &cbShadowObj, sizeof(cbShadowObject)))
		return false;

	gDevCon->VSSetConstantBuffers(0, 1, &gShadowObjBuffer);

	return true;
}

void Spotlight::Release()
{
	gShadowMap->Release();
	gShadowSRV->Release();
	gShadowDSV->Release();

	ShadowMap::Release();
}

void Spotlight::setSpotLight(SpotLightProperties spotLight)
{
	this->spotLight = spotLight;
}

SpotLightProperties Spotlight::getSpotLight() const
{
	return spotLight;
}

ID3D11ShaderResourceView* Spotlight::getSRV() const
{
	return gShadowSRV;
}
