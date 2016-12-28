#include "Camera.h"


Camera::Camera()
{
	// Position
	pos = Vector4(0.0f, 4.0f, -4.0f, 0.0f);
	// What point the camera looks at
	target = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	// For orientation purposes it needs to know what way is up
	up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	// Set the world matrix
	world = XMMatrixIdentity();
	// Set the view matrix
	view = XMMatrixLookAtLH(pos, target, up);
	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WIDTH / HEIGHT, 1.0f, 1000.0f);
}


Camera::~Camera()
{
}

void Camera::InitScene()
{

}

void Camera::Update()
{
}

Matrix Camera::getWorldMatrix() const
{
	return world;
}

Matrix Camera::getViewMatrix() const
{
	return view;
}

Matrix Camera::getProjMatrix() const
{
	return proj;
}
