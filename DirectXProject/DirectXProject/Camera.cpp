#include "Camera.h"


Camera::Camera()
{
	// For orientation purposes it needs to know what way is up, forward, and right
	up = Vector3(0.0f, 1.0f, 0.0f);
	forward = Vector3(0.0f, 0.0f, 1.0f);
	right = Vector3(1.0f, 0.0f, 0.0f);
	// Position
	pos = Vector3(0.0f, 0.0f, -4.0f);

	// Set the world matrix
	world = XMMatrixIdentity();
	// Set the view matrix
	view = XMMatrixLookAtLH(pos, forward, up);
	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WIDTH / HEIGHT, 1.0f, 1000.0f);

	yaw = 90.0f;
	pitch = 0.0f;
}


Camera::~Camera()
{
}

void Camera::InitScene()
{

}

void Camera::Update(bool* keys, Vector2 mouseOffset, float dt)
{
	ProcessKeyboard(keys, dt);
	ProcessMouse(mouseOffset, dt);

	// Update the view matrix
	view = XMMatrixLookAtLH(pos, pos + forward, up);
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

void Camera::ProcessKeyboard(bool * keys, float dt)
{
	float velocity = 10.0f * dt;

	// Change the position according to the key pressed
	if (keys[W])
		pos += forward * velocity;
	if (keys[A])
		pos -= right * velocity;
	if (keys[S])
		pos -= forward * velocity;
	if (keys[D])
		pos += right * velocity;
}

void Camera::ProcessMouse(Vector2 mouseOffset, float dt)
{
	float sensitivity = 1000.0f;

	// Change the yaw and pitch depending on the mouse offset since the last frame
	yaw += mouseOffset.x * sensitivity * dt;
	pitch += mouseOffset.y * sensitivity * dt;

	// Makes sure the camera doesn't get flipped over
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	// Recalculate the vectors
	forward.x = cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch));
	forward.y = sin(XMConvertToRadians(pitch));
	forward.z = sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch));
	forward = XMVector3Normalize(forward);
	right = XMVector3Normalize(XMVector3Cross(Vector3(0.0f, 1.0f, 0.0f), forward));
	up = XMVector3Normalize(XMVector3Cross(forward, right));
}
