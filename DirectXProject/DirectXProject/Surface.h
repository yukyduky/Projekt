#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include "globals.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>





using namespace DirectX;
using namespace DirectX::SimpleMath;

class Surface
{
public:
	Surface();
	~Surface();

	bool InitScene(ID3D11Device* gDevice);
	void Render(ID3D11DeviceContext* gDevCon);
	void Release();
	float* heightValueList();

private:
	// Functions
	bool LoadTextures(ID3D11Device* gDevice);
	bool CreateVertexData(ID3D11Device* gDevice);
	Vector3 CreateVertexNormal(Vector3* pos);
	bool CreateIndexBuffer(ID3D11Device* gDevice);
	bool CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize, int stride);
	Vector4 Surface::CreateTBNMatrixData(Vector3* posTBNData, Vector3 norTBNData, Vector2* uvTBNData);


	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoords;
		Vector4 tangent;
	};

	struct HeightMapData
	{
		int width;
		int height;
		int numFaces;
		float* tempHeightValue;
		Vector3* vertexData;
		Vector3* normals;
		DWORD* indices;
	};

	HeightMapData hmd;
	bool LoadHeightMapInfo();
	bool LoadBMPInfo();
	void CreateHMVertices();
	void CreateNormalBMP(std::vector<Vertex> v, int nrOfVertices);
	

	// COMS
	ID3D11Buffer* gIndexBuffer;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gDiffuseMap;
	ID3D11ShaderResourceView* gNormalMap;

	// Variables
	UINT stride;
	UINT offset;
	float rot;

	// Error handling
	HRESULT hr;
};

#endif