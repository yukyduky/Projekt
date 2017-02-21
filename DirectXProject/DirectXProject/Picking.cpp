#include "Picking.h"

Picking::Picking()
{

}

Picking::~Picking()
{

}

void Picking::pickBoxes(bool keys, Box Boxes[], Camera cam, Matrix boxWorld, ID3D11Device *gDevice)
{

	if (keys & clickOnce)
	{
		for (int i = 0; i < 4; i++)
			Boxes[i].setWorld(boxWorld);
		//Sorts the boxes in distance to camera order.
		const int NumbBoxes = 4;
		float distance[NumbBoxes];
		for (int i = 0; i < NumbBoxes; i++)
			distance[i] = Boxes[i].getCenterVector().Distance(cam.getPosition(), Boxes[i].getCenterVector());
		
		int order[NumbBoxes];
		for (int i = 0; i < NumbBoxes; i++)
		{
			order[i] = i;
		}
		for (int j = 0; j < NumbBoxes; j++)
		{
			float lowest = distance[j];
			int lowestIndex = j;

			for (int i = j; i < NumbBoxes; i++)
				if (distance[i] < lowest)
				{
					lowest = distance[i];
					lowestIndex = i;
				}

			if (order[j] != lowestIndex)
			{
				int temp = order[j];
				order[j] = lowestIndex;
				order[lowestIndex] = temp;
				distance[lowestIndex] = 99999.0f;
			}
		}

		//Checks intersection until it hits
		for (int i = 0; i < NumbBoxes; i++)
		{
 			Box* heldBox = PointBox(&Boxes[order[i]], cam);
			if (heldBox != nullptr)
			{
				heldBox->onClick(gDevice);
				i = NumbBoxes;
			}
		}

		clickOnce = false;
	}
	else if (!keys)
		clickOnce = true;
}

Box* Picking::PointBox(Box* box, Camera cam)
{
	//Puts the vaulues into a Ray for easy handling in the funktions
	Ray ray(cam.getPosition(), cam.getForward());

	if (CheckHit(box->getHitBox(), ray, box->getWidth()))
		return box;
	else
		return nullptr;
}

//If 
Vector2 Picking::GetHits(Plane p1, Plane p2, Ray inRay, float width)
{
	float xValue = intersectionDistance(p1, inRay);

	//xValue will be 9999.0 if the ray is parrallel with the planes
	if (xValue != 9999.0f)
	{
		float yValue = intersectionDistance(p2, inRay);

		//Makes sure that XValue is the lowest
		if (xValue > yValue)
		{
			float temp = yValue;
			yValue = xValue;
			xValue = temp;
		}
		if (yValue > 0.0f)
			return Vector2(xValue, yValue);
		//Value tha twill fail if the intersecton is behind the camera
		else
			return Vector2(9999.0f, -9999.0f);
		
	}
	//Checks if they ray is between the planes
	float d1 = DistanceToPlane(p1, inRay), d2 = DistanceToPlane(p2, inRay);
	if((fabs(d1+d2) < width + 0.001f))
		return Vector2(-9999.0f, 9999.0f);
	else
		return Vector2(9999.0f, -9999.0f);

}

float Picking::DistanceToPlane(Plane plane, Ray inRay)
{
	return 
		(
			(
					plane.Normal().x*inRay.position.x
				+	plane.Normal().y*inRay.position.y 
				+	plane.Normal().z*inRay.position.z 
				+	plane.D()
			)
			/	
			(sqrt
				(
					(plane.Normal().x)*(plane.Normal().x)
				+	(plane.Normal().y)*(plane.Normal().y)
				+	(plane.Normal().z)*(plane.Normal().z)
				)
			)
		);
}

float Picking::intersectionDistance(Plane plane, Ray inRay)
{
	//Fomrula used: t=(-d-(n Dot o)/(n Dot d)
	float temp = plane.Normal().Dot(inRay.direction);
	//To avoid divided by 0;
 	if (fabs(temp) > 0.0000001f)
		return (( - plane.D() - plane.Normal().Dot(inRay.position)) / temp);
	else
		return 9999.0f;
}

bool Picking::CheckHit(Plane *hitBox, Ray inRay, float width)
{
	//front and back
	Vector2 d = GetHits(hitBox[0], hitBox[3], inRay, width);
	float tMin = d.x;
	float tMax = d.y;
	if (tMax < tMin)
		return false;

	//Right and left
	d = GetHits(hitBox[1], hitBox[2], inRay, width);
	if (d.x > tMin)
		tMin = d.x;
	if (d.y < tMax)
		tMax = d.y;
	if (tMax < tMin)
		return false;

	//Top and bottom
	d = GetHits(hitBox[4], hitBox[5], inRay, width);
	if (d.x > tMin)
		tMin = d.x;
	if (d.y < tMax)
		tMax = d.y;
	if (tMax < tMin)
		return false;
	
	return true;
}


//OLD:
//Returns true if the ray hits the triangle vertice
bool Picking::IntersectionCheck(Vector3 vert1, Vector3 vert2, Vector3 vert3, Ray inRay)
{
	Vector3 s1 = vert2 - vert1;
	Vector3 s2 = vert3 - vert1;

	//Normal of the plane
	Vector3 normal = s1.Cross(s2);
	normal.Normalize();

	//The D value of the plane
	float D = -(normal.x * s1.x) - (normal.y * s1.y) - (normal.z * s1.z);

	//The equations to find the distance between the origin of hteRay and the intersection of hte plane
	float e1 = (inRay.position.x * normal.x) + (inRay.position.y * normal.y) + (inRay.position.z * normal.z),
		e2 = (inRay.direction.x * normal.x) + (inRay.direction.y * normal.y) + (inRay.direction.z * normal.z);

	//If e2 is 0 the ray will be parralel with the plane
	if (e2 = 0)
		return false;

	//distance
	float t = -(e1+D)/(e2);

	//Point where the ray intersects
	Vector3 hitPoint = (inRay.position + inRay.direction) * t;

	//The Barycentric method to se if the intersection in in the triangle or not
	Vector3 cp1 = (vert3 - vert2).Cross(hitPoint - vert2);
	Vector3 cp2 = (vert3 - vert2).Cross(vert1 - vert2);
	if ((cp1.Dot(cp2)) >= 0)
	{
		cp1 = (vert3 - vert1).Cross(hitPoint - vert1);
		cp2 = (vert3 - vert1).Cross(vert2 - vert1);
		if ((cp1.Dot(cp2)) >= 0)
		{
			cp1 = (vert2 - vert1).Cross(hitPoint - vert1);
			cp2 = (vert2 - vert1).Cross(vert3 - vert1);
			if ((cp1.Dot(cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}