#include "Surface.h"



Surface::Surface()
{
}

Surface::~Surface()
{
}

bool Surface::InitScene(Vector3 startPos, float width, ID3D11Device* gDevice)
{
	// Create the Vertex data (position, normal, etc) and the vertex buffer
	if (!CreateVertexData(startPos, width, gDevice))
		return false;

	// Create the index buffer for what order to draw the vertices in
	if (!CreateIndexBuffer(gDevice))
		return false;

	// Load the textures into memory
	if (!LoadTextures(gDevice))
		return false;

	return true;
}

void Surface::Render(ID3D11DeviceContext* gDevCon)
{
	// Set the index buffer
	gDevCon->IASetIndexBuffer(gIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the vertex buffer
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &stride, &offset);
	// Set Primitive Topology
	gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Update the pixel shader with the textures
	gDevCon->PSSetShaderResources(0, 1, &gDiffuseMap);
	// Draw the indexed vertices
	gDevCon->DrawIndexed(6, 0, 0);
}

void Surface::Release()
{
	gIndexBuffer->Release();
	gVertBuffer->Release();
	gDiffuseMap->Release();
}

void Surface::setOffset(UINT offset)
{
	this->offset = offset;
}

bool Surface::LoadTextures(ID3D11Device* gDevice)
{
	// Load diffuse map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\bricks\\diffuse.jpg", nullptr, &gDiffuseMap, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Create surface diffuse texture from file - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Surface::CreateVertexData(Vector3 startPos, float width, ID3D11Device* gDevice)
{
	Vector3 position[] =
	{
		Vector3(startPos.x, startPos.y, startPos.z + width),		// TopLeft
		Vector3(startPos.x + width, startPos.y, startPos.z + width),// TopRight
		Vector3(startPos.x, startPos.y, startPos.z),				// BottomLeft
		Vector3(startPos.x + width, startPos.y, startPos.z),		// BottomRight
	};

	const int numVertices = sizeof(position) / sizeof(Vector3);

	Vector3 normal[numVertices];

	for (int i = 0; i < numVertices; i += 4)
	{
		Vector3 pos[3];

		for (int k = 0; k < 3; k++)
			pos[k] = position[i + k];

		for (int k = 0; k < 4; k++)
			normal[i + k] = CreateVertexNormal(pos);
	}

	Vector2 texCoords[] =
	{
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 1.0f),
		Vector2(1.0f, 1.0f),
	};

	Vector3 posTBNData[3];
	Vector3 norTBNData;
	Vector2 uvTBNData[3];

	Vector4 tangent[numVertices];

	// Get the texture coords
	for (int i = 0; i < 3; i++)
		uvTBNData[i] = texCoords[i];

	for (int i = 0; i < numVertices; i += 4)
	{
		// Get the positions
		for (int k = 0; k < 3; k++)
			posTBNData[k] = position[i + k];

		// Get the normal
		norTBNData = normal[i];

		// Calculate the tangent per face
		Vector4 tan = CreateTBNMatrixData(posTBNData, norTBNData, uvTBNData);

		// Store the data per vertex
		for (int k = 0; k < 4; k++)
			tangent[i + k] = tan;
	}

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoords;
		Vector4 tangent;
	};

	Vertex v[numVertices];

	int k = 0;
	for (int i = 0; i < numVertices; i++)
	{
		if (i % 4 == 0 && i != 0)
			k++;

		v[i].position = position[i];
		v[i].normal = normal[i];
		v[i].texCoords = texCoords[i - 4 * k];
		v[i].tangent = tangent[i];
	}

	// Create the vertexbuffer for the input layout to the shader
	if (!CreateVertexBuffer(gDevice, &v, sizeof(v), sizeof(Vertex)))
		return false;

	return true;
}

Vector3 Surface::CreateVertexNormal(Vector3* pos)
{
	Vector3 p0p1 = pos[1] - pos[0];
	Vector3 p0p2 = pos[2] - pos[0];

	Vector3 normal = p0p1.Cross(p0p2);
	normal.Normalize();

	return normal;
}

bool Surface::CreateIndexBuffer(ID3D11Device* gDevice)
{
	// Index of how to draw the vertices
	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		2,  1,  3,
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
	if (FAILED(hr))
	{
		MessageBox(0, "Surface Index Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Surface::CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize, int stride)
{
	// Describe the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexDataSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Set the vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexData;
	memset(&vertexData, 0, sizeof(vertexData));
	vertexData.pSysMem = ptrV;
	hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &gVertBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Vertex Buffer - Failed", "Error", MB_OK);
		return false;
	}

	this->stride = stride;
	this->offset = 0;

	return true;
}

Vector4 Surface::CreateTBNMatrixData(Vector3* posTBNData, Vector3 norTBNData, Vector2* uvTBNData)
{
	// Edges of triangle; position delta
	Vector3 v0v1 = posTBNData[1] - posTBNData[0];
	v0v1.Normalize();
	Vector3 v0v2 = posTBNData[2] - posTBNData[0];
	v0v2.Normalize();
	// Normal
	Vector3 normal = norTBNData;
	// Texture coords delta
	Vector2 uv0uv1 = uvTBNData[1] - uvTBNData[0];
	Vector2 uv0uv2 = uvTBNData[2] - uvTBNData[0];

	// Calculate the determinant in order to get orientation of the surface
	float det = 1 / (uv0uv1.x * uv0uv2.y - uv0uv1.y * uv0uv2.x);

	// Calculate the tangent and the bitangent
	Vector3 tan;
	tan.x = (v0v1.x * uv0uv2.y - v0v2.x * uv0uv1.y) * det;
	tan.y = (v0v1.y * uv0uv2.y - v0v2.y * uv0uv1.y) * det;
	tan.z = (v0v1.z * uv0uv2.y - v0v2.z * uv0uv1.y) * det;
	Vector3 bitangent;
	bitangent.x = (v0v2.x * uv0uv2.x - v0v1.x * uv0uv1.x) * det;
	bitangent.y = (v0v2.y * uv0uv2.x - v0v1.y * uv0uv1.x) * det;
	bitangent.z = (v0v2.z * uv0uv2.x - v0v1.z * uv0uv1.x) * det;

	//if (bitangent.Dot(normal.Cross(Vector3(tangent))) < 0.0f)
	//	(Vector3)tangent *= -1.0f;
	(Vector3)tan = ((Vector3)tan - normal * normal.Dot((Vector3)tan));

	// Normalize after everythings done
	tan.Normalize();

	Vector4 tangent = Vector4(tan.x, tan.y, tan.z, det);

	return tangent;
}
