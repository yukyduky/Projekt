#include "QuadTree.h"


QuadTree::QuadTree(Node node) : node(node)
{
	subdivide();
}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; i++)
		delete node.children[i];
}

bool QuadTree::insert(Vector3 p, std::vector<DWORD>& vertIndexData)
{
	// The node contains the point
	if (this->node.qp.contains(p))
	{
		bool inserted = false;

		if (this->node.type == LEAF)
		{
			inserted = true;

			for (int i = 0; i < 6; i++)
				this->node.vertIndexData.push_back(vertIndexData[i]);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				inserted = this->node.children[i]->insert(p, vertIndexData);
				if (inserted)
					break;
			}
		}
		return inserted;
	}
	return false;
}

bool QuadTree::getDrawData(std::vector<Plane>& frustumPlanes, std::vector<DWORD>& data)
{
	if (this->node.qp.quad == QTOPLEFT)
	{
		if (!checkQuad(this->node.qp.x - this->node.qp.offset, this->node.qp.z + this->node.qp.offset, this->node.qp.offset, frustumPlanes))
			return false;
	}
	else if (this->node.qp.quad == QTOPRIGHT)
	{
		if (!checkQuad(this->node.qp.x, this->node.qp.z + this->node.qp.offset, this->node.qp.offset, frustumPlanes))
			return false;
	}
	else if(this->node.qp.quad == QBOTTOMRIGHT)
	{
		if (!checkQuad(this->node.qp.x, this->node.qp.z, this->node.qp.offset, frustumPlanes))
			return false;
	}
	else if (this->node.qp.quad == QBOTTOMLEFT)
	{
		if (!checkQuad(this->node.qp.x - this->node.qp.offset, this->node.qp.z, this->node.qp.offset, frustumPlanes))
			return false;
	}

	bool stored = false;

	// If the node is a leaf then store the data, otherwise continue down the tree
	if (this->node.type == LEAF)
	{
		for (int i = 0; i < this->node.vertIndexData.size(); i++)
			data.push_back(this->node.vertIndexData[i]);
		stored = true;
	}
	else
	{
		for (int i = 0; i < 4; i++)
			stored = this->node.children[i]->getDrawData(frustumPlanes, data);
	}

	return stored;
}

QuadTree::Node& QuadTree::getNode()
{
	return this->node;
}

void QuadTree::subdivide()
{
	// Continue to subdivide until we reach the last layer
	if (this->node.layer > 1)
	{
		if (this->node.qp.quad == QTOPLEFT)
		{
			for (int i = 0; i < 4; i++)
				this->node.children[i] = new QuadTree(Node(QuadPlane(node.qp.x - node.qp.offset / 2, node.qp.z + node.qp.offset / 2, node.qp.offset / 2, QTOPLEFT + i), node.layer - 1, NODE, node.nodeID + i));
		}
		if (this->node.qp.quad == QTOPRIGHT)
		{
			for (int i = 0; i < 4; i++)
				this->node.children[i] = new QuadTree(Node(QuadPlane(node.qp.x + node.qp.offset / 2, node.qp.z + node.qp.offset / 2, node.qp.offset / 2, QTOPLEFT + i), node.layer - 1, NODE, node.nodeID + i));
		}
		if (this->node.qp.quad == QBOTTOMRIGHT)
		{
			for (int i = 0; i < 4; i++)
				this->node.children[i] = new QuadTree(Node(QuadPlane(node.qp.x + node.qp.offset / 2, node.qp.z - node.qp.offset / 2, node.qp.offset / 2, QTOPLEFT + i), node.layer - 1, NODE, node.nodeID + i));
		}
		if (this->node.qp.quad == QBOTTOMLEFT)
		{
			for (int i = 0; i < 4; i++)
				this->node.children[i] = new QuadTree(Node(QuadPlane(node.qp.x - node.qp.offset / 2, node.qp.z - node.qp.offset / 2, node.qp.offset / 2, QTOPLEFT + i), node.layer - 1, NODE, node.nodeID + i));
		}
	}
	// Once last layer is reached: Set the nodes to the leaf type
	else
	{
		this->node.type = LEAF;
		for (int i = 0; i < 4; i++)
			this->node.children[i] = nullptr;
	}
}

bool QuadTree::checkQuad(float x, float z, float offset, std::vector<Plane> frustumPlanes)
{
	for (int i = 0; i < frustumPlanes.size(); i++)
	{
		int inCount = 4;

		// Top left corner of the quad
		if (frustumPlanes[i].DotCoordinate(Vector3(x, 0.0f, z)) < 0.0f)
			inCount--;
		// Top right corner of the quad
		if (frustumPlanes[i].DotCoordinate(Vector3(x + offset, 0.0f, z)) < 0.0f)
			inCount--;
		// Bottom right corner of the quad
		if (frustumPlanes[i].DotCoordinate(Vector3(x + offset, 0.0f, z - offset)) < 0.0f)
			inCount--;
		// Bottom left corner of the quad
		if (frustumPlanes[i].DotCoordinate(Vector3(x, 0.0f, z - offset)) < 0.0f)
			inCount--;

		// If all the points of a quad was behind one particular plane, then the quad is completely outside of the frustum -> return false without any further checking
		// Any other case is either completely inside or intersecting with the frustum, either way we want to keep them
		if (inCount == 0)
			return false;
	}

	return true;
}
