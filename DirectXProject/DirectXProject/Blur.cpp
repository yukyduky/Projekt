#include "Blur.h"

Blur::Blur()
{
	cbObj.screenWidth = WIDTH;
	cbObj.screenHeight = HEIGHT;
	cbObj.padding1 = 0.0f;
	cbObj.padding2 = 0.0f;
}

Blur::~Blur()
{

}

void Blur::Release()
{
	this->gVertBuffer->Release();
	this->blurSamplerState->Release();
	this->cbObjBuffer->Release();

	this->gHorizontalBlurTex->Release();
	this->gHorizontalBlurSRV->Release();
	this->gHorizontalBlurRTV->Release();

	this->gVerticalBlurTex->Release();
	this->gVerticalBlurSRV->Release();
	this->gVerticalBlurRTV->Release();

	this->gDiffuseGlowTex->Release();
	this->gDiffuseGlowSRV->Release();
	this->gDiffuseGlowRTV->Release();

	this->diffuseGlowShader.Release();
	this->horizontalBlurShader.Release();
	this->verticalBlurShader.Release();
}

bool Blur::InitScene(ID3D11Device* gDevice, ID3D11DeviceContext* gDevCon)
{
	//Create shaders
	if (!this->CreateShaders(gDevice))
		return false;

	//Create Vertex Buffer
	if (!this->CreateVertexBuffer(gDevice))
		return false;

	//Create sampler state
	if (!this->CreateSamplerState(gDevice))
		return false;

	//Bind all the textures
	if (!this->BindTextures(gDevice))
		return false;

	//Create Constant Buffer for BlurVertex shader
	if (!this->CreateConstantBuffer(gDevice, &this->cbObjBuffer, sizeof(cbObject)))
		return false;

	//Map cbuffer
	if (!this->MapBuffer(gDevCon, &this->cbObjBuffer, &this->cbObj, sizeof(this->cbObj)))
		return false;

	return true;
}

bool Blur::CreateShaders(ID3D11Device* gDevice)
{
	//Create horizontalBlurShaders
	if (!horizontalBlurShader.CreateShaders(gDevice, this->fileNameHorizontalBlurVertex, this->fileNameHorizontalBlurPixel, this->inputDesc, BLUR_INPUT_DESC_SIZE))
		return false;
	//Create verticalBlurShaders
	if (!verticalBlurShader.CreateShaders(gDevice, this->fileNameVerticalBlurVertex, this->fileNameVerticalBlurPixel, this->inputDesc, BLUR_INPUT_DESC_SIZE))
		return false;
	//Create diffuseGlowShader
	if (!diffuseGlowShader.CreateShaders(gDevice, this->fileNameDiffuseGlowPassVertex, this->fileNameDiffuseGlowPassPixel, this->inputDesc, BLUR_INPUT_DESC_SIZE))
		return false;

	return true;
}

bool Blur::BindTextures(ID3D11Device* gDevice)
{
	//Bind blur SRV and RTV to blur texture
	if (!this->BindTextureToRTVAndSRV(gDevice, &this->gDiffuseGlowTex, &this->gDiffuseGlowRTV, &this->gDiffuseGlowSRV))
		return false;

	//Bind blur SRV and RTV to blur texture
	if (!this->BindTextureToRTVAndSRV(gDevice, &this->gHorizontalBlurTex, &this->gHorizontalBlurRTV, &this->gHorizontalBlurSRV))
		return false;

	//Bind blur SRV and RTV to blur texture
	if (!this->BindTextureToRTVAndSRV(gDevice, &this->gVerticalBlurTex, &this->gVerticalBlurRTV, &this->gVerticalBlurSRV))
		return false;

	return true;
}

void Blur::SetDiffuseGlowPassShaders(ID3D11DeviceContext* gDevCon)
{
	this->diffuseGlowShader.SetShaders(gDevCon);
}

void Blur::SetHorizontalBlurShaders(ID3D11DeviceContext* gDevCon)
{
	this->horizontalBlurShader.SetShaders(gDevCon);
}

void Blur::SetVerticalBlurShaders(ID3D11DeviceContext* gDevCon)
{
	this->verticalBlurShader.SetShaders(gDevCon);
}

bool Blur::CreateVertexBuffer(ID3D11Device* gDevice)
{
	struct Vertex
	{
		float x, y, z, w;
		float u, v;
	};

	Vertex v[] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
	};
	
	// Describe the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Set the vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexData;
	memset(&vertexData, 0, sizeof(vertexData));
	vertexData.pSysMem = &v;

	this->hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &gVertBuffer);
	if (FAILED(this->hr))
	{
		MessageBox(0, "Create Vertex buffer(Blur) - Failed", "Error", MB_OK);
		return false;
	}

	this->vertBufferStride = sizeof(Vertex);
	this->vertBufferOffset = 0;

	return true;
}

void Blur::SetVertexBuffer(ID3D11DeviceContext* gDevCon)
{
	gDevCon->IASetVertexBuffers(0, 1, &gVertBuffer, &vertBufferStride, &vertBufferOffset);
}	

bool Blur::CreateSamplerState(ID3D11Device* gDevice)
{
	D3D11_SAMPLER_DESC SamplerDesc;
	memset(&SamplerDesc, 0, sizeof(D3D11_SAMPLER_DESC));
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	this->hr = gDevice->CreateSamplerState(&SamplerDesc, &blurSamplerState);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Samplerstate (BlurSampler) - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void Blur::SetSamplerState(ID3D11DeviceContext* gDevCon)
{
	gDevCon->PSSetSamplers(0, 1, &this->blurSamplerState);
}

bool Blur::CreateConstantBuffers(ID3D11Device* gDevice)
{
	D3D11_BUFFER_DESC cbBufferDesc;
	memset(&cbBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	cbBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbBufferDesc.ByteWidth = 0;
	cbBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBufferDesc.MiscFlags = 0;

	this->hr = gDevice->CreateBuffer(&cbBufferDesc, nullptr, &this->gVertBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Constant Buffer (Blur) - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

bool Blur::BindTextureToRTVAndSRV(ID3D11Device* gDevice, ID3D11Texture2D** gTexture, ID3D11RenderTargetView** gRTV, ID3D11ShaderResourceView** gSRV)
{
	// Describe the texture
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.CPUAccessFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create one texture per rendertarget
	hr = gDevice->CreateTexture2D(&texDesc, nullptr, gTexture);
	if (FAILED(hr))
	{
		MessageBox(0, "Create blur texture - Failed", "Error", MB_OK);
		return false;
	}
	

	// Describe the Rendertargetview
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create one rtv per output from the pixel shader
	hr = gDevice->CreateRenderTargetView(*gTexture, &rtvDesc, gRTV);
	if (FAILED(hr))
	{
		MessageBox(0, "Create blur RTV - Failed", "Error", MB_OK);
		return false;
	}

	// Describe the Shaderresourceview
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create one srv per texture to be loaded into the next pixel shader
	hr = gDevice->CreateShaderResourceView(*gTexture, &srvDesc, gSRV);
	if (FAILED(hr))
	{
		MessageBox(0, "Create blur SRV - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void Blur::SetHorizontalBlurRTV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->OMSetRenderTargets(1, &this->gHorizontalBlurRTV, NULL);
}

void Blur::SetHorizontalBlurSRV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->PSSetShaderResources(7, 1, &this->gHorizontalBlurSRV);
}

void Blur::SetDiffuseGlowRTV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->OMSetRenderTargets(1, &this->gDiffuseGlowRTV, NULL);
}

void Blur::SetDiffuseGlowSRV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->PSSetShaderResources(7, 1, &this->gDiffuseGlowSRV);
}

void Blur::SetVerticalBlurRTV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->OMSetRenderTargets(1, &this->gVerticalBlurRTV, NULL);
}

void Blur::SetVerticalBlurSRV(ID3D11DeviceContext* gDevCon)
{
	gDevCon->PSSetShaderResources(7, 1, &this->gVerticalBlurSRV);
}

bool Blur::CreateConstantBuffer(ID3D11Device* gDevice, ID3D11Buffer** gBuffer, int bufferSize)
{
	D3D11_BUFFER_DESC cbBufferDesc;
	memset(&cbBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	cbBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbBufferDesc.ByteWidth = bufferSize;
	cbBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBufferDesc.MiscFlags = 0;

	this->hr = gDevice->CreateBuffer(&cbBufferDesc, nullptr, gBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Create Constant Buffer (Blur) - Failed", "Error", MB_OK);
		return false;
	}
	return true;
}

bool Blur::MapBuffer(ID3D11DeviceContext* gDevCon, ID3D11Buffer** gBuffer, void* cbPtr, int structSize)
{
	// Map constant buffer so that we can write to it.
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	hr = gDevCon->Map(*gBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	if (FAILED(hr))
	{
		MessageBox(0, "Deferred Buffer mapping - Failed", "Error", MB_OK);
		return false;
	}
	// copy memory from CPU to GPU the entire struct
	memcpy(dataPtr.pData, cbPtr, structSize);
	// UnMap constant buffer so that we can use it again in the GPU
	gDevCon->Unmap(*gBuffer, 0);

	return true;
}