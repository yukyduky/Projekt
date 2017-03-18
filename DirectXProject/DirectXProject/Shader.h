#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Shader
{
public:
	Shader();
	Shader(bool tess);
	~Shader();

	void SetShaders(ID3D11DeviceContext* gDevCon);
	bool CreateShaders(ID3D11Device* gDevice, const wchar_t* fileNameVertex, const wchar_t* fileNamePixel, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize);
	bool CreateTessShaders(ID3D11Device* gDevice, const wchar_t* fileNameHull, const wchar_t* fileNameDomain, const wchar_t * fileNameGeometry);
	void Release();

private:
	bool CreateVertexShader(ID3D11Device* gDevice, const wchar_t* fileName, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize);
	bool CreatePixelShader(ID3D11Device* gDevice, const wchar_t* fileName);
	bool CreateHullShader(ID3D11Device* gDevice, const wchar_t* fileName);
	bool CreateDomainShader(ID3D11Device* gDevice, const wchar_t* fileName);
	bool CreateGeometryShader(ID3D11Device * gDevice, const wchar_t * fileName);
	bool CreateInputLayout(ID3D11Device* gDevice, ID3DBlob* pVS, const D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescSize);
	void SetVertexShader(ID3D11DeviceContext* gDevCon) const;
	void SetPixelShader(ID3D11DeviceContext* gDevCon) const;
	void SetHullShader(ID3D11DeviceContext* gDevCon) const;
	void SetDomainShader(ID3D11DeviceContext* gDevCon) const;
	void SetGeometryShader(ID3D11DeviceContext * gDevCon) const;
	void SetInputlayout(ID3D11DeviceContext* gDevCon) const;

	// COMs
	ID3D11VertexShader* gVertexShader;
	ID3D11PixelShader* gPixelShader;
	ID3D11HullShader* gHullShader = nullptr;
	ID3D11DomainShader* gDomainShader = nullptr;
	ID3D11GeometryShader* gGeometyrShader;
	ID3D11InputLayout * gVertexLayout;

	//Shader uses Tesselation
	bool tesselation = false;

	// Error handling
	HRESULT hr;
};

#endif
