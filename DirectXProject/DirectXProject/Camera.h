#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	~Camera();
	
	void InitScene();
	void Update(bool* keys, Vector2 mouseOffset, float dt);

	Matrix getViewMatrix() const;
	Matrix getProjMatrix() const;
	Vector3 getPosition() const;
	Vector3 getForward() const;

private:
	// Functions
	void ProcessKeyboard(bool* keys, float dt);
	void ProcessMouse(Vector2 mouseOffset, float dt);

	// Matrices
	Matrix view;
	Matrix proj;

	// Vectors
	Vector3 up;
	Vector3 forward;
	Vector3 right;
	Vector3 pos;

	// Variables
	float yaw;
	float pitch;
};

#endif
