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
	void Update();

	Matrix getWorldMatrix() const;
	Matrix getViewMatrix() const;
	Matrix getProjMatrix() const;


private:
	// Matrices
	Matrix world;
	Matrix view;
	Matrix proj;

	// Vectors
	Vector4 pos;
	Vector4 target;
	Vector4 up;
};

#endif
