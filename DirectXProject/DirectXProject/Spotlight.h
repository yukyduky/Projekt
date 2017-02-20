#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "ShadowMap.h"

//Structs
struct SpotLightProperties
{
	Matrix shadowMatrix;
	Vector3 pos;
	float pad0;
	Vector3 dir;
	float angle;
	Vector4 diffuse;
	Vector3 ambient;
	float pad1;
	Vector3 attenuation;
	float specPower;
};

class Spotlight : public ShadowMap
{
public:
	Spotlight();
	~Spotlight();

	bool InitScene(ID3D11Device* gDevice);
	void Update();
	bool Render(ID3D11DeviceContext* gDevCon);
	bool UpdateResources(ID3D11DeviceContext* gDevCon);
	void Release();

	void setSpotLight(SpotLightProperties spotLight);
	SpotLightProperties getSpotLight() const;
	ID3D11ShaderResourceView* getSRV() const;

private:
	// COMS
	ID3D11Texture2D* gShadowMap;
	ID3D11ShaderResourceView* gShadowSRV;
	ID3D11DepthStencilView* gShadowDSV;
	ID3D11Buffer* gShadowObjBuffer;

	// Matrices
	Matrix shadowMatrix;

	// Vectors
	Vector3 up;
	Vector3 right;

	// Objects
	SpotLightProperties spotLight;
};

#endif
