#pragma once
#ifndef BOX_H
#define BOX_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Box
{
public:
	Box();
	~Box();

	bool LoadTextures(ID3D11Device* gDevice);
	bool InitScene(ID3D11Device* gDevice);
	void Update(float dt);
	void Render(ID3D11DeviceContext* gDevCon);
	void Release();

	Matrix getWorldMatrix() const;

private:
	// Functions
	bool CreateVertexData(ID3D11Device* gDevice);
	bool CreateIndexBuffer(ID3D11Device* gDevice);
	bool CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize);
	void CreateTBNMatrix(Vector3 posTBNData);

	// COMS
	ID3D11Buffer* gIndexBuffer;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gDiffuseMap;
	ID3D11ShaderResourceView* gNormalMap;

	// Matrices
	Matrix world;
	Matrix rotate;

	// Variables
	UINT stride;
	UINT offset;
	float rot;

	// Error handling
	HRESULT hr;

};

#endif
