#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Model
{
public:
	Model();

	virtual bool PositionData(ID3D11Device* gDevice) = 0;
	virtual bool SetTexture(ID3D11Device* gDevice) = 0;
	virtual bool InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(ID3D11DeviceContext* gDevCon) = 0;
	virtual void Release() = 0;

	virtual Matrix getWorldMatrix() const = 0;

protected:
	Matrix world;
};

#endif
