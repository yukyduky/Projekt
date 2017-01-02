#pragma once
#ifndef BOX_H
#define BOX_H

#include "Model.h"

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
