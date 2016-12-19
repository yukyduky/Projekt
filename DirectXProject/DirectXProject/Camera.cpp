#include "Camera.h"


Camera::Camera()
{
	pos = Vector4(0.0f, 4.0f, -4.0f, 0.0f);
	target = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::InitScene()
{
	// Set the world matrix
	world = XMMatrixIdentity();
	// Set the view matrix
	view = XMMatrixLookAtLH(pos, target, up);
	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WIDTH / HEIGHT, 1.0f, 1000.0f);
	// Combine the matrices
	wvp = world * view * proj;
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

Matrix Camera::getWVPMatrix() const
{
	return wvp;
}
