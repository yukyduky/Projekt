#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Light
{
public:
	Light(int lightType, Vector4 lightPosition);
	Light(int lightType, Vector4 lightPosition, Vector4 lightDirection);
	Light(int lightType, Vector4 lightPosition, Vector4 lightDirection, float attenuation);
	~Light();

	int getLightType();
	

private:
	int lightType;
	Vector4 lightPosition;
	Vector4 lightDirection;
	float attenuation;
};

#endif
