#pragma once
#ifndef FRUSTUMCULLING_H
#define FRUSTUMCULLING_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


class FrustumCulling
{
public:
	FrustumCulling();
	~FrustumCulling();

private:

	struct Point
	{
		Vector2 p;
		Point(Vector2 p) : p{ p.x, p.y } {};
	};

	struct AABB
	{

	};
};

#endif
