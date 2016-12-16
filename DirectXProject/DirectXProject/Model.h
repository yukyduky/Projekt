#pragma once
#ifndef MODEL
#define MODEL_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Model
{
public:
	Model();
	~Model();

	virtual bool Position() = 0;
	virtual bool Texture() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	Matrix world;
};

#endif
