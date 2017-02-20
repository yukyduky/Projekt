#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"
#include "Box.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Surface.h"
#include "Spotlight.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct PointLight
{
	Vector3 pos;
	float pad0;
	Vector4 diffuse;
	Vector3 ambient;
	float pad1;
	Vector3 attenuation;
	float specPower;
};

/*struct SpotLight
{
	Vector3 pos;
	float pad0;
	Vector3 dir;
	float angle;
	Vector4 diffuse;
	Vector3 ambient;
	float pad1;
	Vector3 attenuation;
	float specPower;
};*/

struct DirectLight
{
	Vector3 pos;
	float pad0;
	Vector3 dir;
	float pad1;
	Vector4 diffuse;
	Vector3 ambient;
	float specPower;
};

struct GeneralLightAttrb
{
	Vector3 cameraPos;
	float pad;
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	// Initialize the scene
	bool InitScene(ID3D11Device* gDevice);
	// Update
	void Update();
	// Render Box
	bool Render(ID3D11DeviceContext* gDevCon);
	bool CreateShadowMap(ID3D11DeviceContext * gDevCon, ID3D11ShaderResourceView** gSpotShadowMap);
	// Release the memory
	void Release();

	Matrix getMatrixWVP() const;
	Matrix getMatrixWorld() const;
	PointLight getPointLight() const;

private:
	//COMS 
	ID3D11Buffer* gGeoObjBuffer;
	ID3D11Buffer* gLightLightBuffer;
	ID3D11SamplerState* gShadowSampler;

	// Functions
	bool CreateConstBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize);
	bool MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer ** gBuffer, void * cbPtr, int structSize);
	void UpdateWorlds();
	void UpdateBox();
	void UpdateFlashLight(Vector3 position, Vector3 forward);

	// Matrices
	Matrix staticWorld;
	Matrix view;
	Matrix proj;
	Matrix wvp;
	Matrix boxWorld;
	
	// Vectors
	Vector2 mouseOffset;

	// Objects
	Box box;
	Surface surface;
	Box box2;
	Camera cam;
	Time time;
	Input input;
	Spotlight spotLight;

	// Variables
	float dt;
	bool keys[NUM_KEYS];
	float rotBoxY;
	float rotBoxZ;
	float transBox;
	float camPreviousHeight;

	// Structs
	PointLight pointLight;
	//SpotLight spotLight;
	DirectLight directLight;
	GeneralLightAttrb genLight;

	// GeoShader Constant buffer: Object
	struct cbGeoObject
	{
		Matrix world;
		Matrix wvp;
	};

	// LightShader Constant buffer: Light
	struct cbLightLighting
	{
		PointLight pointLight;
		SpotLightProperties spotLight;
		DirectLight directLight;
		GeneralLightAttrb genLight;
	};

	// Constant Buffers
	cbGeoObject cbGeoObj;
	cbLightLighting cbLightLight;

	// Error handling
	HRESULT hr;
};

#endif
