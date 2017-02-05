#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "ShadowMap.h"


class Pointlight : public ShadowMap
{
public:
	Pointlight();
	~Pointlight();

	bool BindTextureToDSVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexure, ID3D11DepthStencilView** gDSV, ID3D11ShaderResourceView** gSRV);


private:
	// Structs
	struct PointLight
	{
		Matrix vp;
		Vector3 pos;
		float pad0;
		Vector4 diffuse;
		Vector3 ambient;
		float pad1;
		Vector3 attenuation;
		float specPower;
	};

	PointLight pointLight;

	// Error handling
	HRESULT hr;
};

#endif
