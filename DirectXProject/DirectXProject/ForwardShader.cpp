#include "ForwardShader.h"



ForwardShader::ForwardShader()
{
}


ForwardShader::~ForwardShader()
{
}

void ForwardShader::InitScene(ID3D11DeviceContext * gDevCon)
{
	SetVertexShader(gDevCon);
	SetFragShader(gDevCon);
	SetInputlayout(gDevCon);
}

bool ForwardShader::CreateShaders(ID3D11Device * gDevice)
{
	// Create Vertex Shader
	if (!CreateVertexShader(gDevice))
	{
		MessageBox(0, "Vertex Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}

	// Create Fragment Shader
	if (!CreateFragmentShader(gDevice))
	{
		MessageBox(0, "Fragment Shader Initialization - Failed", "Error", MB_OK);
		return false;
	}
}

void ForwardShader::SetVertexShader(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->VSSetShader(gVertexShader, nullptr, 0);
}

void ForwardShader::SetFragShader(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->PSSetShader(gFragShader, nullptr, 0);
}

void ForwardShader::SetInputlayout(ID3D11DeviceContext * gDevCon) const
{
	gDevCon->IASetInputLayout(gVertexLayout);
}

void ForwardShader::Release()
{
	gVertexShader->Release();
	gFragShader->Release();
	gVertexLayout->Release();
}

bool ForwardShader::CreateVertexShader(ID3D11Device * gDevice)
{
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"vertex.hlsl",		// filename
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

	hr = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Vertex Shader - Failed", "Error", MB_OK);
		return false;
	}

	if (!CreateInputLayout(gDevice, pVS))
		return false;
	
	pVS->Release();

	return true;
}

bool ForwardShader::CreateFragmentShader(ID3D11Device * gDevice)
{
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"fragment.hlsl",	// filename
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

	hr = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gFragShader);
	if (FAILED(hr))
	{
		MessageBox(0, "Fragment Shader - Failed", "Error", MB_OK);
		return false;
	}
		
	pPS->Release();

	return true;
}

bool ForwardShader::CreateInputLayout(ID3D11Device* gDevice, ID3DBlob* pVS)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(inputDesc) / sizeof(float) / 7;

	// Create the Input Layout
	hr = gDevice->CreateInputLayout(inputDesc, 3, pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(0, "Input Layout - Failed", "Error", MB_OK);
		return false;
	}

	return true;
}
