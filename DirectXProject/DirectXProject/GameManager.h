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

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct LightProperties
{
	Vector3 pos;
	float range;
	Vector3 dir;
	float spotlightAngle;
	Vector4 diffuse;
	Vector3 ambient;
	float pad1;
	Vector3 attenuation;
	float pad2;
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
	// Render
	bool Render(ID3D11DeviceContext* gDevCon);
	// Release the memory
	void Release();

	Matrix getMatrixWVP() const;
	Matrix getMatrixWorld() const;
	LightProperties getLightProperties() const;

private:
	// Functions
	void UpdateWorlds();
	void UpdateBox();

	// Matrices
	Matrix world;
	Matrix view;
	Matrix proj;
	Matrix wvp;
	Matrix boxWorld;
	
	// Vectors
	Vector2 mouseOffset;

	// Objects
	Box box;
	//ForwardShader shaderForward;
	Camera cam;
	Time time;
	Input input;

	// Variables
	float dt;
	bool keys[NUM_KEYS];
	float rotBoxY;
	float rotBoxZ;
	float transBox;

	LightProperties pointLight;

	// Error handling
	HRESULT hr;
};

#endif
