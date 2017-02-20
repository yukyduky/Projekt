#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Surface
{
public:
	Surface();
	~Surface();

	bool InitScene(Vector3 startPos, float width, ID3D11Device* gDevice);
	void Render(ID3D11DeviceContext* gDevCon);
	void Release();

	void setOffset(UINT offset);

private:
	// Functions
	bool LoadTextures(ID3D11Device* gDevice);
	bool CreateVertexData(Vector3 startPos, float width, ID3D11Device* gDevice);
	Vector3 CreateVertexNormal(Vector3* pos);
	bool CreateIndexBuffer(ID3D11Device* gDevice);
	bool CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize, int stride);
	Vector4 Surface::CreateTBNMatrixData(Vector3* posTBNData, Vector3 norTBNData, Vector2* uvTBNData);

	// COMS
	ID3D11Buffer* gIndexBuffer;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gDiffuseMap;

	// Variables
	UINT stride;
	UINT offset;
	float rot;

	// Error handling
	HRESULT hr;
};

#endif
