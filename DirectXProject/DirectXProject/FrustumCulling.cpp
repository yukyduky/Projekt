#include "FrustumCulling.h"


FrustumCulling::FrustumCulling(float nearDist, float farDist, float fov, float ar) : 
	qt(QuadTree::Node(QuadTree::QuadPlane(256, 256, 256, QBOTTOMLEFT), 8, NODE, 0)), nearDist(nearDist), farDist(farDist)
{
	halfWidthNearPlane = nearDist * tan(XMConvertToRadians(fov / 2.0f));
	halfHeightNearPlane = halfWidthNearPlane / ar;
	halfWidthFarPlane = farDist * tan(XMConvertToRadians(fov / 2.0f));
	halfHeightFarPlane = halfWidthFarPlane / ar;
}

FrustumCulling::~FrustumCulling()
{
}

void FrustumCulling::CalcFrustum(Vector3 pos, Vector3 forward, Vector3 right)
{
	Vector3 up = XMVector3Normalize(forward.Cross(right));
	Vector3 nCenter = pos + forward * nearDist;
	Vector3 fCenter = pos + forward * farDist;
	
	Plane aPlane;
	frustumPlanes.clear();

	// Creates all the frustum planes
	// Near plane
	aPlane = Plane(nCenter + up * halfHeightNearPlane, nCenter, nCenter + right * halfWidthNearPlane);
	frustumPlanes.push_back(aPlane);
	// Far plane
	aPlane = Plane(fCenter, fCenter + up * halfHeightNearPlane, fCenter + right * halfWidthNearPlane);
	frustumPlanes.push_back(aPlane);
	// Right plane
	aPlane = Plane(pos + right * halfWidthNearPlane, fCenter - up * halfHeightFarPlane + right * halfWidthFarPlane * 1.4f, pos + right * halfWidthNearPlane + up);
	frustumPlanes.push_back(aPlane);
	// Left plane
	aPlane = Plane(fCenter - up * halfHeightFarPlane - right * halfWidthFarPlane * 1.4f, pos - right * halfWidthNearPlane, pos - right * halfWidthNearPlane + up);
	frustumPlanes.push_back(aPlane);
	// Bottom plane
	aPlane = Plane(pos - up * halfHeightFarPlane, nCenter - up * halfHeightFarPlane + right * halfWidthFarPlane, pos - up * halfHeightFarPlane + right * halfWidthNearPlane);
	frustumPlanes.push_back(aPlane);
	// Top plane
	aPlane = Plane(pos + up * halfHeightFarPlane * 1.4f, nCenter + up * halfHeightFarPlane * 1.4f - right * halfWidthNearPlane, pos + up * halfHeightFarPlane * 1.4f - right * halfWidthNearPlane);
	frustumPlanes.push_back(aPlane);
}

void FrustumCulling::FillQuadTree(std::vector<std::vector<Vector3>>& p, std::vector<DWORD>& data)
{
	for (int i = 0; i < p.size() - 1; i++)
	{
		for (int j = 0; j < p[0].size() - 1; j++)
		{
			std::vector<DWORD> pointData;

			// Prepare what data we are about to insert into the QuadTree
			// Add 6 index values for 2 triangles per vertex except for the bottom row and the right most row
			for (int k = 0; k < 6; k++)
				pointData.push_back(data[(i * (p.size() - 1) * 6) + (j * 6) + k]);

			for (int k = 0; k < 4; k++)
			{
				// Check the QuadTree for the correct location for the data and store it
				if (qt.getNode().children[k]->insert(p[i][j], pointData))
					break;
			}
		}
	}
}

std::vector<DWORD> FrustumCulling::getDrawData()
{
	this->drawData.clear();

	// Check the QuadTree using the frustumplanes
	for (int i = 0; i < 4; i++)
		qt.getNode().children[i]->getDrawData(frustumPlanes, drawData);

	return this->drawData;
}
