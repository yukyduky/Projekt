#pragma once
#ifndef PICKING_H
#define PICKING_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "globals.h"
#include "Box.h"
#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Picking
{
public:
	Picking();
	~Picking();

	void pickBoxes(bool keys, Box Boxes[], Camera cam, Matrix boxWorld, ID3D11Device *gDevice);

private:
	bool clickOnce = false;

	//Returns a box if it was a hit
	Box* PointBox(Box* box, Camera cam);
	bool CheckHit(Plane* hitBox, Ray inRay, float width);

	//A v2 with intersectiondistance to planes
	Vector2 GetHits(Plane p1, Plane p2, Ray inRay, float width);

	//Math functions
	float intersectionDistance(Plane plane, Ray inRay);
	float DistanceToPlane(Plane plane, Ray inRay);


	//OLD:
	//For triangles
	bool IntersectionCheck(Vector3 vert1, Vector3 vert2, Vector3 vert3, Ray inRay);
};
#endif