#pragma once
#ifndef FORWARDSHADER_H
#define FORWARDSHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>

class ForwardShader
{
public:
	ForwardShader();
	~ForwardShader();

	void InitScene(ID3D11DeviceContext* gDevCon);
	bool CreateShaders(ID3D11Device* gDevice);
	void Release();

private:
	bool CreateVertexShader(ID3D11Device* gDevice);
	bool CreateFragmentShader(ID3D11Device* gDevice);
	bool CreateInputLayout(ID3D11Device* gDevice, ID3DBlob* pVS);
	void SetVertexShader(ID3D11DeviceContext* gDevCon) const;
	void SetFragShader(ID3D11DeviceContext* gDevCon) const;
	void SetInputlayout(ID3D11DeviceContext* gDevCon) const;

	// COMs
	ID3D11VertexShader* gVertexShader;
	ID3D11PixelShader* gFragShader;
	ID3D11InputLayout * gVertexLayout;

	// Error handling
	HRESULT hr;
};

#endif
