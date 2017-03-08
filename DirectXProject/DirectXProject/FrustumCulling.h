#pragma once
#ifndef FRUSTUMCULLING_H
#define FRUSTUMCULLING_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "globals.h"
#include "QuadTree.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


class FrustumCulling
{
public:
	FrustumCulling(float nearDist, float farDist, float fov, float ar);
	~FrustumCulling();

	void CalcFrustum(Vector3 pos, Vector3 forward, Vector3 up);
	void FillQuadTree(std::vector<std::vector<Vector3>>& p, std::vector<DWORD>& data);

	std::vector<DWORD> getDrawData();

private:
	// Variables
	float nearDist;
	float farDist;
	float halfWidthNearPlane;
	float halfHeightNearPlane;
	float halfWidthFarPlane;
	float halfHeightFarPlane;
	std::vector<DWORD> drawData;
	std::vector<Plane> frustumPlanes;

	// Objects
	QuadTree qt;

};

#endif
