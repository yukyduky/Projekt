#pragma once
#ifndef BOX_H
#define BOX_H

#include "Model.h"

class Box : public Model
{
public:
	Box();
	~Box();

	virtual bool PositionData(ID3D11Device* gDevice);
	virtual bool SetTexture(ID3D11Device* gDevice);
	virtual bool InitScene(ID3D11DeviceContext* gDevCon, ID3D11Device* gDevice);
	virtual void Update(float dt);
	virtual void Render(ID3D11DeviceContext* gDevCon);
	virtual void Release();

	virtual Matrix getWorldMatrix() const;

private:
	// COMS
	ID3D11Buffer* gIndexBuffer;
	ID3D11Buffer* gVertBuffer;
	ID3D11ShaderResourceView* gTextureRV;

	// Matrices
	Matrix rotate;

	// Variables
	UINT stride;
	UINT offset;
	float rot;

	// Error handling
	HRESULT hr;

};

#endif
