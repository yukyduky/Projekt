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

	bool InitScene(ID3D11Device* gDevice, Vector3 startPos, float width);
	void Render(ID3D11DeviceContext* gDevCon);
	void Release();
	
	void setOffset(UINT offset);

private:
	// Functions
	bool LoadTextures(ID3D11Device* gDevice);
	bool CreateVertexData(ID3D11Device* gDevice, Vector3 startPos, float width); //Startpoint = frontface bottom left
	Vector3 CreateVertexNormal(Vector3* pos);
	bool CreateIndexBuffer(ID3D11Device* gDevice);
	bool CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize, int stride);
	Vector4 CreateTBNMatrixData(Vector3* posTBNData, Vector3 norTBNData, Vector2* uvTBNData);

	// COMS
	ID3D11Buffer* gIndexBuffer;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gDiffuseMap;
	ID3D11ShaderResourceView* gNormalMap;
	ID3D11ShaderResourceView* gSpecularMap;

	// Variables
	UINT stride;
	UINT offset;

	// Error handling
	HRESULT hr;

};

#endif
