#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "globals.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


class QuadTree
{
public:
	// Structs
	struct QuadPlane
	{
		Vector3 xNor = Vector3(1.0f, 0.0f, 0.0f);
		Vector3 zNor = Vector3(0.0f, 0.0f, 1.0f);
		int quad;
		float x;
		float z;
		float offset;

		QuadPlane(float x, float z, float offset, float quad) : x(x), z(z), offset(offset), quad(quad) {};
		QuadPlane(Vector3 c, float quad) : x(c.x), z(c.z), quad(quad) {};

		bool contains(Vector3 p)
		{
			// If dot product is positive against the x-plane and negative against the z-plane and we are in the correct node
			if (p.Dot(xNor) - x >= 0 && p.Dot(zNor) - z >= 0 && quad == QTOPRIGHT)
				return true;
			else if (p.Dot(xNor) - x <= 0 && p.Dot(zNor) - z >= 0 && quad == QTOPLEFT)
				return true;
			else if (p.Dot(xNor) - x >= 0 && p.Dot(zNor) - z <= 0 && quad == QBOTTOMRIGHT)
				return true;
			else if (p.Dot(xNor) - x <= 0 && p.Dot(zNor) - z <= 0 && quad == QBOTTOMLEFT)
				return true;
			
			return false;
		}
	};

	struct Node
	{
		QuadTree* children[4];
		std::vector<DWORD> vertIndexData;
		int type;
		int layer;
		int nodeID;
		QuadPlane qp;

		Node(QuadPlane qp, int layer, int type, int nodeID) :
			qp(qp), layer(layer), type(type), nodeID(nodeID) {};
	};

	QuadTree(Node node);
	~QuadTree();

	bool insert(Vector3 p, std::vector<DWORD>& vertIndexData);
	bool getDrawData(std::vector<Plane>& frustumPlanes, std::vector<DWORD>& data);
	Node& getNode();

private:
	// Functions
	void subdivide();
	bool checkQuad(float x, float z, float offset, std::vector<Plane> frustumPlanes);


	// Objects
	Node node;
};

#endif
