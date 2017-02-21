#include "Box.h"


Box::Box()
{
}

Box::~Box()
{
}

bool Box::InitScene(ID3D11Device* gDevice, Vector3 startPos, float width)
{
	// Create the Vertex data (position, normal, etc) and the vertex buffer
	if (!CreateVertexData(gDevice, startPos, width))
		return false;

	// Create the index buffer for what order to draw the vertices in
	if (!CreateIndexBuffer(gDevice))
		return false;

	// Load the textures into memory
	if (!LoadTextures(gDevice))
		return false;

	return true;
}

void Box::Render(ID3D11DeviceContext* gDevCon)
{
	// Set the index buffer
	gDevCon->IASetIndexBuffer(gIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the vertex buffer
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &stride, &offset);
	// Set Primitive Topology
	gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Update the pixel shader with the textures
	gDevCon->PSSetShaderResources(0, 1, &gDiffuseMap);
	gDevCon->PSSetShaderResources(1, 1, &gNormalMap);
	gDevCon->PSSetShaderResources(2, 1, &gSpecularMap);
	// Draw the indexed vertices
	gDevCon->DrawIndexed(36, 0, 0);
}

void Box::Release()
{
	gIndexBuffer->Release();
	gVertBuffer->Release();
	gDiffuseMap->Release();
	gNormalMap->Release();
	gSpecularMap->Release();
}

void Box::setOffset(UINT offset)
{
	this->offset = offset;
}

bool Box::LoadTextures(ID3D11Device* gDevice)
{
	// Load diffuse map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\crate2\\diffuse.png", nullptr, &gDiffuseMap, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Create box diffuse texture from file - Failed", "Error", MB_OK);
		return false;
	}

	// Load normal map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\crate\\normal.png", nullptr, &gNormalMap, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Create box normal texture from file - Failed", "Error", MB_OK);
		return false;
	}

	// Load normal map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\crate2\\specular.png", nullptr, &gSpecularMap, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Create box specular texture from file - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Box::CreateVertexData(ID3D11Device* gDevice, Vector3 startPos, float width)
{
	this->width = width;

	Vector3 position[] =
	{
		// Front Face
		Vector3(startPos.x, startPos.y + width, startPos.z),				// TopLeft
		Vector3(startPos.x + width, startPos.y + width, startPos.z),		// TopRight
		startPos,															// BottomLeft
		Vector3(startPos.x + width, startPos.y, startPos.z),				// BottomRight

		// Right Face
		Vector3(startPos.x + width, startPos.y + width, startPos.z),		// TopLeft
		Vector3(startPos.x + width, startPos.y + width, startPos.z + width),// TopRight
		Vector3(startPos.x + width, startPos.y, startPos.z),				// BottomLeft
		Vector3(startPos.x + width, startPos.y, startPos.z + width),		// BottomRight

		// Left Face
		Vector3(startPos.x, startPos.y + width, startPos.z + width),		// TopLeft
		Vector3(startPos.x, startPos.y + width, startPos.z),				// TopRight
		Vector3(startPos.x, startPos.y, startPos.z + width),				// BottomLeft
		startPos,															// BottomRight

		// Back Face
		Vector3(startPos.x + width, startPos.y + width, startPos.z + width),// TopLeft
		Vector3(startPos.x, startPos.y + width, startPos.z + width),		// TopRight
		Vector3(startPos.x + width, startPos.y, startPos.z + width),		// BottomLeft
		Vector3(startPos.x, startPos.y, startPos.z + width),				// BottomRight

		// Top Face
		Vector3(startPos.x, startPos.y + width, startPos.z + width),		// TopLeft
		Vector3(startPos.x + width, startPos.y + width, startPos.z + width),// TopRight
		Vector3(startPos.x, startPos.y + width, startPos.z),				// BottomLeft
		Vector3(startPos.x + width, startPos.y + width, startPos.z),		// BottomRight

		// Bottom Face
		startPos,															// TopLeft
		Vector3(startPos.x + width, startPos.y, startPos.z),				// TopRight
		Vector3(startPos.x, startPos.y, startPos.z + width),				// BottomLeft
		Vector3(startPos.x + width, startPos.y, startPos.z + width),		// BottomRight
	};


	center = Vector3(startPos.x + (width / 2), startPos.y + (width / 2), startPos.z + (width / 2));

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

	this->normals[0] = normal[0];//front	4*0
	this->normals[1] = normal[4];//right	4*1
	this->normals[2] = normal[16];//top		4*4

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

Vector3 Box::CreateVertexNormal(Vector3* pos)
{
	Vector3 p0p1 = pos[1] - pos[0];
	Vector3 p0p2 = pos[2] - pos[0];

	Vector3 normal = p0p1.Cross(p0p2);
	normal.Normalize();

	return normal;
}

bool Box::CreateIndexBuffer(ID3D11Device* gDevice)
{
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
	if (FAILED(hr))
	{
		MessageBox(0, "Box Index Buffer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Box::CreateVertexBuffer(ID3D11Device* gDevice, void* ptrV, int vertexDataSize, int stride)
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

Vector4 Box::CreateTBNMatrixData(Vector3* posTBNData, Vector3 norTBNData, Vector2* uvTBNData)
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

void Box::onClick(ID3D11Device* gDevice)
{
	if (textureSwitch)
	{ 
		// Load diffuse map texture from file
		hr = CreateWICTextureFromFile(gDevice, L"Textures\\smiley\\diffuse.png", nullptr, &gDiffuseMap, NULL);
		if (hr != S_OK)
		{
			MessageBox(0, "Changing texure with onClick() - Failed", "Error", MB_OK);
		}

		// Load normal map texture from file
		hr = CreateWICTextureFromFile(gDevice, L"Textures\\smiley\\normal.png", nullptr, &gNormalMap, NULL);
		if (hr != S_OK)
		{
			MessageBox(0, "Changing normalmap with onClick - Failed", "Error", MB_OK);
		}
	}
	else
	{
		// Load diffuse map texture from file
		hr = CreateWICTextureFromFile(gDevice, L"Textures\\crate2\\diffuse.png", nullptr, &gDiffuseMap, NULL);
		if (hr != S_OK)
		{
			MessageBox(0, "Changing back texure with onClick() - Failed", "Error", MB_OK);
		}

		// Load normal map texture from file
		hr = CreateWICTextureFromFile(gDevice, L"Textures\\crate\\normal.png", nullptr, &gNormalMap, NULL);
		if (hr != S_OK)
		{
			MessageBox(0, "Changing back normalmap with onClick - Failed", "Error", MB_OK);
		}
	}
	textureSwitch = !textureSwitch;
}

Plane* Box::getHitBox()
{
	Vector3 center = this->center.Transform(this->center, world), 
		normals[3];
	for (int i = 0; i < 3; i++)
		normals[i] = this->normals[i].Transform(this->normals[i], world);

	//Creates the 6 planes that will make up the hitbox
	//The order will be: Front, Right, Left, Back, Top, Bottom
	tempHitBox[0] = Plane(		// Front Face
		center + normals[0] * (width / 2), normals[0]);

	tempHitBox[1] = Plane(		// Right Face
		center + normals[1] * (width / 2), normals[1]);

	tempHitBox[2] = Plane(		// Left Face
		center - normals[1] * (width / 2), normals[1]);

	tempHitBox[3] = Plane(		// Back Face
		center - normals[0] * (width / 2), normals[0]);

	tempHitBox[4] = Plane(		// Top Face
		center + normals[2] * (width / 2), normals[2]);

	tempHitBox[5] = Plane(		// Bottom Face
		center - normals[2] * (width / 2), normals[2]);

	return tempHitBox;
}