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
	void Update(bool* keys, Vector2 mouseOffset, float dt, float* heightValues);

	Matrix getViewMatrix() const;
	Matrix getProjMatrix() const;
	Vector3 getPosition() const;
	Vector3 getForward() const;
	Vector3 getRight() const;
	float getNearDist() const;
	float getFarDist() const;
	float getFOV() const;
	float getAR() const;

private:
	// Functions
	void ProcessKeyboard(bool* keys, float dt);
	void ProcessMouse(Vector2 mouseOffset, float dt);

	// Matrices
	Matrix view;
	Matrix proj;
	//New
	Matrix rotationMatrix;
	Matrix groundWorld;


	// Vectors
	Vector3 up;
	Vector3 worldUp;
	Vector3 forward;
	Vector3 worldForward;
	Vector3 right;
	Vector3 worldRight;
	Vector3 pos;
	Vector3 target;

	// Variables
	float yaw;
	float pitch;
	float nearD;
	float farD;
	float fov;
	float ar;
	
	int flyMode;
	float moveLR;
	float moveBF;
	
};

#endif
