#include "Shader.h"



Shader::Shader()
{
}
Shader::Shader(bool tess)
{
	tesselation = tess;
}


Shader::~Shader()
{
}

void Shader::SetShaders(ID3D11DeviceContext * gDevCon)
{
	SetVertexShader(gDevCon);
	SetPixelShader(gDevCon);
	SetInputlayout(gDevCon);

	//TESSELATION
	if (true)
	{
		SetHullShader(gDevCon);	
		SetDomainShader(gDevCon);
		SetGeometryShader(gDevCon);
	}
}

bool Shader::CreateShaders(ID3D11Device* gDevice, const wchar_t* fileNameVertex, const wchar_t* fileNamePixel, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize)
{
	// Create Vertex Shader
	if (!CreateVertexShader(gDevice, fileNameVertex, inputDesc, inputDescSize))
	{
		MessageBox(0, "Vertex Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	// Create Pixel Shader
	if (!CreatePixelShader(gDevice, fileNamePixel))
	{
		MessageBox(0, "Pixel Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	tesselation = false;

	return true;
}

bool Shader::CreateTessShaders(ID3D11Device * gDevice, const wchar_t * fileNameHull, const wchar_t * fileNameDomain, const wchar_t * fileNameGeometry)
{
	// Create Hull Shader
	if (!CreateHullShader(gDevice, fileNameHull))
	{
		MessageBox(0, "Hull Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	// Create Domain Shader
	if (!CreateDomainShader(gDevice, fileNameDomain))
	{
		MessageBox(0, "Domain Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	// Create Geometry Shader
	if (!CreateGeometryShader(gDevice, fileNameGeometry))
	{
		MessageBox(0, "Geometry Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	tesselation = true;

	return true;
}

void Shader::Release()
{
	gVertexShader->Release();
	gPixelShader->Release();
	gHullShader->Release();
	gDomainShader->Release();
	gGeometyrShader->Release();
	gVertexLayout->Release();
}

bool Shader::CreateVertexShader(ID3D11Device* gDevice, const wchar_t* fileName, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize)
{
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		fileName,		// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"VS",				// entry point
		"vs_5_0",			// shader model (target)
		0,					// shader compile options			// here DEBUGGING OPTIONS
		0,					// effect compile options
		&pVS,				// double pointer to ID3DBlob		
		nullptr				// pointer for Error Blob messages.
							// how to use the Error blob, see here
							// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);

	// Create Vertex shader
	hr = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Vertex Shader - Failed", "Error", MB_OK);
		return false;
	}

	// Create input layout
	if (!CreateInputLayout(gDevice, pVS, inputDesc, inputDescSize))
		return false;

	pVS->Release();

	return true;
}

bool Shader::CreatePixelShader(ID3D11Device* gDevice, const wchar_t* fileName)
{
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		fileName,			// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"PS",				// entry point
		"ps_5_0",			// shader model (target)
		0,					// shader compile options
		0,					// effect compile options
		&pPS,				// double pointer to ID3DBlob		
		nullptr				// pointer for Error Blob messages.
							// how to use the Error blob, see here
							// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);

	// Create Pixel shader
	hr = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Pixel Shader - Failed", "Error", MB_OK);
		return false;
	}

	pPS->Release();

	return true;
}

bool Shader::CreateHullShader(ID3D11Device * gDevice, const wchar_t * fileName)
{
	ID3DBlob* pHS = nullptr;
	D3DCompileFromFile(
		fileName,			// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"HS",				// entry point
		"hs_5_0",			// shader model (target)
		0,					// shader compile options
		0,					// effect compile options
		&pHS,				// double pointer to ID3DBlob		
		nullptr				// pointer for Error Blob messages.
							// how to use the Error blob, see here
							// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	// Create Pixel shader
	hr = gDevice->CreateHullShader(pHS->GetBufferPointer(), pHS->GetBufferSize(), nullptr, &gHullShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Hull Shader - Failed", "Error", MB_OK);
		return false;
	}

	pHS->Release();

	return true;
}

bool Shader::CreateDomainShader(ID3D11Device * gDevice, const wchar_t * fileName)
{
	ID3DBlob* pDS = nullptr;
	D3DCompileFromFile(
		fileName,			// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"DS",				// entry point
		"ds_5_0",			// shader model (target)
		0,					// shader compile options
		0,					// effect compile options
		&pDS,				// double pointer to ID3DBlob		
		nullptr				// pointer for Error Blob messages.
							// how to use the Error blob, see here
							// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	// Create Pixel shader
	hr = gDevice->CreateDomainShader(pDS->GetBufferPointer(), pDS->GetBufferSize(), nullptr, &gDomainShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Domain Shader - Failed", "Error", MB_OK);
		return false;
	}

	pDS->Release();

	return true;
}

bool Shader::CreateGeometryShader(ID3D11Device * gDevice, const wchar_t * fileName)
{
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		fileName,			// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"GS",				// entry point
		"gs_5_0",			// shader model (target)
		0,					// shader compile options
		0,					// effect compile options
		&pGS,				// double pointer to ID3DBlob		
		nullptr				// pointer for Error Blob messages.
							// how to use the Error blob, see here
							// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	// Create Pixel shader
	hr = gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometyrShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Geometry Shader - Failed", "Error", MB_OK);
		return false;
	}

	pGS->Release();

	return true;
}

bool Shader::CreateInputLayout(ID3D11Device* gDevice, ID3DBlob* pVS, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize)
{
	// Create the Input Layout
	hr = gDevice->CreateInputLayout(inputDesc, inputDescSize, pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(0, "Input Layout - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void Shader::SetVertexShader(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->VSSetShader(gVertexShader, nullptr, 0);
}

void Shader::SetPixelShader(ID3D11DeviceContext* gDevCon) const
{
	gDevCon->PSSetShader(gPixelShader, nullptr, 0);
}

void Shader::SetHullShader(ID3D11DeviceContext* gDevCon) const
{
	gDevCon->HSSetShader(gHullShader, nullptr, 0);
}

void Shader::SetDomainShader(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->DSSetShader(gDomainShader, nullptr, 0);
}

void Shader::SetGeometryShader(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->GSSetShader(gGeometyrShader, nullptr, 0);
}

void Shader::SetInputlayout(ID3D11DeviceContext* gDevCon) const
{
	gDevCon->IASetInputLayout(gVertexLayout);
}
