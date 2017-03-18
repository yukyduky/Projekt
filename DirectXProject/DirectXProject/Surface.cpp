#include "Surface.h"



Surface::Surface()
{
	this->hmd.tempHeightValue = nullptr;
	this->hmd.normals = nullptr;
}

Surface::~Surface()
{
	if (this->hmd.tempHeightValue != nullptr)
	{
		delete[] this->hmd.tempHeightValue;
		this->hmd.tempHeightValue = nullptr;
	}

	if (this->hmd.normals != nullptr) 
	{
		delete[] this->hmd.normals;
		this->hmd.normals = nullptr;
	}
}

bool Surface::InitScene(ID3D11Device* gDevice)
{
	//Load heightmap
	if (!this->LoadHeightMapInfo())
		return false;

	// Create the index buffer for what order to draw the vertices in
	if (!CreateIndexBuffer(gDevice))
		return false;

	// Create the Vertex data (position, normal, etc) and the vertex buffer
	if (!CreateVertexData(gDevice))
		return false;

	// Load the textures into memory
	if (!LoadTextures(gDevice))
		return false;

	CreateRasteriserState(gDevice);

	return true;
}

void Surface::Render(ID3D11DeviceContext* gDevCon)
{
	// Set the index buffer
	gDevCon->IASetIndexBuffer(gIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the vertex buffer
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &stride, &offset);
	// Set Primitive Topology
	gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	// Update the pixel shader with the textures
	gDevCon->PSSetShaderResources(0, 1, &gDiffuseMap);
	gDevCon->PSSetShaderResources(1, 1, &gNormalMap);
	// Draw the indexed vertices
	gDevCon->DrawIndexed(this->hmd.numFaces * 3, 0, 0);
}

void Surface::Release()
{
	gIndexBuffer->Release();
	gVertBuffer->Release();
	gDiffuseMap->Release();
	//gNormalMap->Release();
}

bool Surface::LoadTextures(ID3D11Device* gDevice)
{
	// Load diffuse map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\surface\\surface.png", nullptr, &gDiffuseMap, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Create surface diffuse texture from file - Failed", "Error", MB_OK);
		return false;
	}
	/*
	// Load normal map texture from file
	hr = CreateWICTextureFromFile(gDevice, L"Textures\\surface\\normalmapreal.png", nullptr, &gNormalMap, NULL);
	if (hr != S_OK)
	{
		MessageBox(0, "Create surface normal texture from file - Failed", "Error", MB_OK);
		return false;
	}
	*/
	return true;
}

bool Surface::CreateVertexData(ID3D11Device* gDevice)
{
	//Calculate the number of vertices we will have depending on the width and height of the heightmap we use
	int nrOfVertices = this->hmd.height * this->hmd.width;

	std::vector<Vertex> v(nrOfVertices); 
	int index = 0;
	

	//Open up the txt file with the precalculated normal if it exists, else we create calculate the normals and save them to the txt file.

	std::string name = "Textures\\surface\\normals.txt";
	std::ifstream f(name.c_str());

	if (f.good())
	{
		std::fstream normalFile("Textures\\surface\\normals.txt", std::ios_base::in);

		for (int i = 0; i < this->hmd.height; i++)
		{
			for (int j = 0; j < this->hmd.width; j++)
			{
				v[index].position = this->hmd.vertexData[i][j];
				//v[index].position.y = 25.5f;
				
				normalFile >> v[index].normal.x;
				getchar();
				normalFile >> v[index].normal.y;
				getchar();
				normalFile >> v[index].normal.z;
				getchar();
				
				v[index].texCoords.x = (float)j / ((float)this->hmd.width - 1); //Give each vertex a tex-coordinate between 0 - 1.0 on the x-axis
				v[index].texCoords.y = (float)i / ((float)this->hmd.height - 1); //Give each vertex a tex-coordinate between 0 - 1.0 on the y-axis
				v[index].tangent = Vector4(1.0f, 0.0f, 0.0f, 0.0f); //Assign each vertex a standard tangent. (Borde fixas ordentligt om vi får tid)

				index++;
			}
		}
		//close the normalFile
		normalFile.close();

	}
	else
	{
		for (int i = 0; i < this->hmd.height; i++)
		{
			for (int j = 0; j < this->hmd.width; j++)
			{
				v[index].position = this->hmd.vertexData[i][j];
				//v[index].position.y = 25.5f;
				v[index].texCoords.x = (float)j / ((float)this->hmd.width - 1); //Give each vertex a tex-coordinate between 0 - 1.0 on the x-axis
				v[index].texCoords.y = (float)i / ((float)this->hmd.height - 1); //Give each vertex a tex-coordinate between 0 - 1.0 on the y-axis
				v[index].tangent = Vector4(1.0f, 0.0f, 0.0f, 0.0f); //Assign each vertex a standard tangent. (Borde fixas ordentligt om vi får tid)

				index++;
			}
		}

		this->CreateNormalBMP(v, nrOfVertices);

		std::fstream normalFile("Textures\\surface\\normals.txt", std::ios_base::in);
		index = 0;

		for (int i = 0; i < this->hmd.height; i++)
		{
			for (int j = 0; j < this->hmd.width; j++)
			{
				normalFile >> v[index].normal.x;
				getchar();
				normalFile >> v[index].normal.y;
				getchar();
				normalFile >> v[index].normal.z;
				getchar();

				index++;
			}
		}
		//close the normalFile
		normalFile.close();
	}
	// Create the vertexbuffer for the input layout to the shader
	if (!CreateVertexBuffer(gDevice, &v[0], sizeof(Vertex)*nrOfVertices, sizeof(Vertex)))
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
	for (int i = 0; i < this->hmd.height - 1; i++)
	{
		for (int j = 0; j < this->hmd.width - 1; j++)
		{
			hmd.indices.push_back((i * (this->hmd.width)) + j);
			hmd.indices.push_back((i + 1) * (this->hmd.width) + j);
			hmd.indices.push_back(i * (this->hmd.width) + j + 1);

			hmd.indices.push_back(i * (this->hmd.width) + j + 1);
			hmd.indices.push_back((i + 1) * (this->hmd.width) + j);
			hmd.indices.push_back((i + 1) * (this->hmd.width) + j + 1);
		}
	}

	// Describe the index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = hmd.indices.size() * sizeof(DWORD);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Set the index buffer data
	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = this->hmd.indices.data();
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

bool Surface::LoadHeightMapInfo()
{
	if (!this->LoadBMPInfo())
		return false;

	this->CreateHMVertices();
	


	return true;
}

bool Surface::LoadBMPInfo()
{
	FILE* filePtr = nullptr;
	fopen_s( &filePtr,"Textures\\surface\\heightmap.bmp" ,"rb");
	if (filePtr == nullptr)
	{
		MessageBox(0, "Surface Load BMP - Failed", "Error", MB_OK);
		return false;
	}

	unsigned char info[54]; //header information
	fread(info, sizeof(unsigned char), 54, filePtr); //reading the header information

	//saving the width and height from header information.
	this->hmd.width = *(int*)&info[18];
	this->hmd.height = *(int*)&info[22];


	int paddedRowLength = (this->hmd.width * 3 + 3) &(~3); 
	unsigned char* data = new unsigned char[paddedRowLength];

	int index = 0;
	hmd.tempHeightValue = new float[this->hmd.width * this->hmd.height];
	
	
	for (int i = 0; i < this->hmd.height; i++)
	{
		index = 0;
		fread(data, sizeof(unsigned char), paddedRowLength, filePtr);
		for (int j = 0; j < this->hmd.width * 3; j += 3)
		{
			this->hmd.tempHeightValue[(this->hmd.width * (this->hmd.height - 1 - i)) + index] = (float)data[j];
			index++;
		}
	}

	delete[] data;
	data = nullptr;

	return true;
}

void Surface::CreateHMVertices() //Creates the vertice points for the heightmap.
{
	//The number of vertices used is determined by the height and width of the heightmap we use
	int index = 0;
	
	//Calculate the number of faces
	this->hmd.numFaces = (this->hmd.width - 1) * (this->hmd.height - 1) * 2;

	//Save the vertex coordinates to hmd, we divide the y-value (the height) with 10.0f to make the surface look a bit smoother
	for (int i = 0; i < this->hmd.height; i++)
	{
		std::vector<Vector3> temp;

		for (int j = 0; j < this->hmd.width; j++)
		{
			temp.push_back(Vector3(j, this->hmd.tempHeightValue[index] / 10.0f, i));
			index++;
		}
		this->hmd.vertexData.push_back(temp);
	}
}

void Surface::CreateNormalBMP(std::vector<Vertex> v, int nrOfVertices)
{
	/*// Thinking in quads in the coming code with the corners numbered as illustrated bellow.
	//   0________1
	//    |      |
	//    |      |
	//    |______|
	//   2        3

	//Calculating the normals
	std::vector<Vector3> temporaryNormal;

	this->hmd.normals = new Vector3[this->hmd.numFaces];

	Vector3 unNormalized = Vector3(0.0f, 0.0f, 0.0f);
	float vecX, vecY, vecZ;
	Vector3 edge1 = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 edge2 = Vector3(0.0f, 0.0f, 0.0f);

	/////////////////////////////////////////////////
	for (int i = 0; i < this->hmd.numFaces / 2; i++)
	{
	//Get the vector describing one edge of our triangle (edge 0,2)
	vecX = v[this->hmd.indices[(i * 6)]].position.x - v[this->hmd.indices[(i * 6) + 2]].position.x;
	vecY = v[this->hmd.indices[(i * 6)]].position.y - v[this->hmd.indices[(i * 6) + 2]].position.y;
	vecZ = v[this->hmd.indices[(i * 6)]].position.z - v[this->hmd.indices[(i * 6) + 2]].position.z;
	edge1 = Vector3(vecX, vecY, vecZ);    //Create our first edge

	//Get the vector describing another edge of our triangle (edge 0,1)
	vecX = v[this->hmd.indices[(i * 6)]].position.x - v[this->hmd.indices[(i * 6) + 1]].position.x;
	vecY = v[this->hmd.indices[(i * 6)]].position.y - v[this->hmd.indices[(i * 6) + 1]].position.y;
	vecZ = v[this->hmd.indices[(i * 6)]].position.z - v[this->hmd.indices[(i * 6) + 1]].position.z;
	edge2 = Vector3(vecX, vecY, vecZ);    //Create our second edge

	//Cross multiply the two edge vectors to get the un-normalized face normal
	unNormalized = XMVector3Cross(edge2, edge1);
	temporaryNormal.push_back(unNormalized);            //Save unormalized normal (for normal averaging)


	//Get the vector describing one edge of our triangle (edge 3,1)
	vecX = v[this->hmd.indices[(i+1) * 6 - 1]].position.x - v[this->hmd.indices[(i+1) * 6 - 2]].position.x;
	vecY = v[this->hmd.indices[(i+1) * 6 - 1]].position.y - v[this->hmd.indices[(i+1) * 6 - 2]].position.y;
	vecZ = v[this->hmd.indices[(i+1) * 6 - 1]].position.z - v[this->hmd.indices[(i+1) * 6 - 2]].position.z;
	edge1 = Vector3(vecX, vecY, vecZ);    //Create our first edge

	//Get the vector describing another edge of our triangle (edge 3,2)
	vecX = v[this->hmd.indices[(i+1) * 6 - 1]].position.x - v[this->hmd.indices[(i+1) * 6 - 3]].position.x;
	vecY = v[this->hmd.indices[(i+1) * 6 - 1]].position.y - v[this->hmd.indices[(i+1) * 6 - 3]].position.y;
	vecZ = v[this->hmd.indices[(i+1) * 6 - 1]].position.z - v[this->hmd.indices[(i+1) * 6 - 3]].position.z;
	edge2 = Vector3(vecX, vecY, vecZ);    //Create our second edge

	//Cross multiply the two edge vectors to get the un-normalized face normal
	unNormalized = XMVector3Cross(edge2, edge1);
	temporaryNormal.push_back(unNormalized);            //Save unormalized normal (for normal averaging)

	}


	//Compute vertex normals (normal Averaging)
	Vector3 normalSum = Vector3(0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tempX;
	float tempY;
	float tempZ;
	std::ofstream normalFile;
	normalFile.open("Textures\\surface\\normals.txt");


	//Go through each vertex
	for (int i = 0; i < nrOfVertices; ++i)
	{
	//Check which triangles use this vertex
	for (int j = 0; j < this->hmd.numFaces; ++j)
	{
	if (this->hmd.indices[j * 3] == i ||
	this->hmd.indices[(j * 3) + 1] == i ||
	this->hmd.indices[(j * 3) + 2] == i)
	{
	tempX = normalSum.x + temporaryNormal[j].x;
	tempY = normalSum.y + temporaryNormal[j].y;
	tempZ = normalSum.z + temporaryNormal[j].z;

	normalSum = Vector3(tempX, tempY, tempZ);    //If a face is using the vertex, add the face normal to the normalSum
	facesUsing++;
	}
	}

	//Get the average normal of this vertex by dividing the sum with the number of faces using it.
	normalSum = normalSum / facesUsing;

	//Normalize the normalSum vector
	normalSum.Normalize();

	//Store the normal in our current vertex 
	v[i].normal = normalSum;
	this->hmd.normals[i] = normalSum;
	
	//Save it to a txt - file so we don't have to calculate it everytime the application runs
	normalFile << normalSum.x;
	normalFile << "\n";
	normalFile << normalSum.y;
	normalFile << "\n";
	normalFile << normalSum.z;
	normalFile << "\n";

	//Clear normalSum and facesUsing for next vertex
	normalSum = Vector3(0.0f, 0.0f, 0.0f);
	facesUsing = 0;
	}

	//close normalFile since all the normals have been saved at this point.
	normalFile.close();
	
	///////////////////////////////////////////////////////////////////////////
	//Save the normals to a BMP so we can use it as a normalmap
	char* data;
	data = new char[this->hmd.width * this->hmd.height * 3];

	float tester;

	int counter = 0;
	for (int i = 0; i < this->hmd.width * this->hmd.height; i++)
	{
		
		if (v[i].normal.z < 0.0f)
		{

		}


		data[counter] = (char)(((v[i].normal.z + 1.0f) / 2) * 255.0f);
		data[counter+1] = (char)(((v[i].normal.y + 1.0f) / 2)* 255.0f);
		data[counter+2] = (char)(((v[i].normal.x + 1.0f) / 2) * 255.0f);
		counter += 3;
	}
	int fileSize = this->hmd.height * this->hmd.width * 3 + 14 + 40;

	char* temp = (char*)((void*)&fileSize);
	char* temp2 = (char*)((void*)&this->hmd.width);
	char* temp3 = (char*)((void*)&this->hmd.height);


	char bmpFileHeader[14] = { 'B', 'M', temp[0],temp[1],temp[2],temp[3], 0,0, 0,0, 54,0,0,0 };
	char bmpFileInfoHeader[40] = { 40,0,0,0, temp2[0],temp2[1],temp2[2],temp2[3], temp3[0],temp3[1],temp3[2],temp3[3], 1,0, 24,0,
									0,0,0,0, 0,0,0,0, 24,0,0,0, 24,0,0,0, 0,0,0,0, 0,0,0,0};
	

	std::ofstream bmpFile;

	bmpFile.open("Textures\\surface\\normals.bmp", std::ios::binary);

	bmpFile.write(bmpFileHeader, 14);
	bmpFile.write(bmpFileInfoHeader, 40);
	bmpFile.write(data, this->hmd.width*this->hmd.height*3);

	bmpFile.close();

	delete[] data;
	data = NULL;

	///////////////////////////////////////////////////////////////////////////////////*/
}

bool Surface::CreateRasteriserState(ID3D11Device* gDevice)
{
	D3D11_RASTERIZER_DESC rastDesc;
	memset(&rastDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0;
	rastDesc.SlopeScaledDepthBias = 0;

	hr = gDevice->CreateRasterizerState(&rastDesc, &rasterizer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Default Rasterizer - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

float* Surface::heightValueList()
{
	return this->hmd.tempHeightValue;
}

bool Surface::setIndexBuffer(ID3D11Device* gDevice, std::vector<DWORD> indices)
{
	if (indices != std::vector<DWORD>())
	{
		hmd.indices = indices;

		// Describe the index buffer
		D3D11_BUFFER_DESC indexBufferDesc;
		memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = indices.size() * sizeof(DWORD);
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		// Set the index buffer data
		D3D11_SUBRESOURCE_DATA indexData;

		indexData.pSysMem = indices.data();
		hr = gDevice->CreateBuffer(&indexBufferDesc, &indexData, &gIndexBuffer);
		if (FAILED(hr))
		{
			MessageBox(0, "Surface Set Index Buffer - Failed", "Error", MB_OK);
			return false;
		}
	}
}

std::vector<DWORD>& Surface::getIndexData()
{
	return this->hmd.indices;
}

std::vector<std::vector<Vector3>>& Surface::getVertexData()
{
	return this->hmd.vertexData;
}
