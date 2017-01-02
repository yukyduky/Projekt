#include "Box.h"


Box::Box()
{
	rot = 0;
}

Box::~Box()
{
}

bool Box::PositionData(ID3D11Device* gDevice)
{
	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
		float nx, ny, nz;
		float u, v;
	};

	Vertex v[] =
	{	
		// Vertices
		// ---Position---|---------Color---------|-------Normal-------|--Texture--
		// Front Face
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,		// TopLeft
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,		// TopRight
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,		// BottomLeft
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,		// BottomRight

		// Right Face
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,		// TopLeft
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,			// TopRight
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,		// BottomLeft
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,		// BottomRight

		// Left Face
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f,		// TopLeft
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,		// TopRight
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f,		// BottomLeft
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 1.0f,	// BottomRight

		// Back Face
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,			// TopLeft
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,		// TopRight
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,		// BottomLeft
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,		// BottomRight

		// Top Face
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,		// TopLeft
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,			// TopRight
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,		// BottomLeft
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// BottomRight

		// Bottom Face
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,		// TopLeft
		1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,		// TopRight
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,		// BottomLeft
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,		// BottomRight
	};

	// Index of how to draw the vertices
	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		2,  1,  3,

		// Right Face
		4,  5,  6,
		6,  5,  7,

		// Left Face
		8,  9, 10,
		10, 9, 11,

		// Back Face
		12, 13, 14,
		14, 13, 15,

		// Top Face
		16, 17, 18,
		18, 17, 19,

		// Bottom Face
		20, 21, 22,
		22, 21, 23
	};

	// Describe the index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices) * sizeof(DWORD);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Set the index buffer data
	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices;
	hr = gDevice->CreateBuffer(&indexBufferDesc, &indexData, &gIndexBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Index Buffer - Failed", "Error", MB_OK);
		return false;
	}

	// Describe the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Set the vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexData;

	memset(&vertexData, 0, sizeof(vertexData));
	vertexData.pSysMem = v;
	hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &gVertBuffer);
	if (hr != S_OK)
	{
		MessageBox(0, "Vertex Buffer - Failed", "Error", MB_OK);
		return false;
	}

	stride = sizeof(Vertex);
	offset = 0;

	return true;
}

bool Box::LoadTextures(ID3D11Device* gDevice)
{
	// Load diffuse map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\ducttape\\diffuse.png", nullptr, &gDiffuseMap, NULL);
	if (hr != S_OK)
	{
		MessageBox(0, "Create texture from file - Failed", "Error", MB_OK);
		return false;
	}

	// Load normal map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\ducttape\\normal.png", nullptr, &gNormalMap, NULL);
	if (hr != S_OK)
	{
		MessageBox(0, "Create texture from file - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Box::InitScene(ID3D11Device* gDevice)
{
	if (!PositionData(gDevice))
		return false;

	if (!LoadTextures(gDevice))
		return false;

	return true;
}

void Box::Update(float dt)
{
	rot += 1 * dt;
	if (rot >= 6.28)
		rot = 0;

	rotate = rotate.CreateRotationY(rot);

	world = rotate;
}

void Box::Render(ID3D11DeviceContext* gDevCon)
{
	// Set the index buffer
	gDevCon->IASetIndexBuffer(gIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the vertex buffer
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &stride, &offset);
	// Set Primitive Topology
	gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Update the pixel shader with the texures
	gDevCon->PSSetShaderResources(0, 1, &gDiffuseMap);
	gDevCon->PSSetShaderResources(1, 1, &gNormalMap);
	// Draw the indexed vertices
	gDevCon->DrawIndexed(36, 0, 0);
}

void Box::Release()
{
	gIndexBuffer->Release();
	gVertBuffer->Release();
	gDiffuseMap->Release();
	gNormalMap->Release();
}

Matrix Box::getWorldMatrix() const
{
	return this->world;
}
