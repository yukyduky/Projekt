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
	// Directionallight
	Light(int lightType, Vector3 lightPosition);
	// Pointlight
	Light(int lightType, Vector3 lightPosition, Vector3 lightDirection, float attenuation);
	// Spotlight
	Light(int lightType, Vector3 lightPosition, Vector3 lightDirection, float attenuation, float angle);
	~Light();

	int getLightType();
	

private:
	int lightType;
	Vector3 lightPosition;
	Vector3 lightDirection;
	float attenuation;
	float angle;
};

#endif
