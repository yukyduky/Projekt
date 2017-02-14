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

	void pickBoxes(bool keys, Box Boxes[], Camera cam, ID3D11Device *gDevice);

private:
	bool clickOnce = false;


	Box* PointBox(Box* box, Camera cam);
	
	//
	Vector2 GetHits(Plane p1, Plane p2, Ray inRay, float width);

	float DistanceToPlane(Plane plane, Ray inRay);

	//Gets the distance from the camera to the Box
	float intersectionDistance(Plane plane, Ray inRay);

	bool CheckHit(Plane hitBox[], Ray inRay, float width);

	//For triangles
	bool IntersectionCheck(Vector3 vert1, Vector3 vert2, Vector3 vert3, Ray inRay);

};
#endif