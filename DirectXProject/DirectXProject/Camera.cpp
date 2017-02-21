#include "Camera.h"


Camera::Camera()
{
	// For orientation purposes it needs to know what way is up, forward, and right
	up = Vector3(0.0f, 1.0f, 0.0f);
	forward = Vector3(0.0f, 0.0f, 1.0f);
	right = Vector3(1.0f, 0.0f, 0.0f);
	// Position
	pos = Vector3(0.0f, 2.0f, -2.0f);

	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(0.4f * XM_PI, (float)WIDTH / HEIGHT, 0.01f, 1000.0f);

	yaw = 0.0f;
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

Matrix Camera::getViewMatrix() const
{
	return this->view;
}

Matrix Camera::getProjMatrix() const
{
	return this->proj;
}

Vector3 Camera::getPosition() const
{
	return this->pos;
}

Vector3 Camera::getForward() const
{
	return this->forward;
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
	float sensitivity = -10000.0f;

	// Change the yaw and pitch depending on the mouse offset since the last frame
	yaw += mouseOffset.x * sensitivity * dt;
	pitch += mouseOffset.y * sensitivity * dt;

	// Makes sure the camera doesn't get flipped over
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	// Create the rotation matrix
	Matrix rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), 0.0f);

	// Recalculate the vectors
	forward = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 0.0f, 1.0f), rotation));
	right = XMVector3Normalize(XMVector3TransformCoord(Vector3(1.0f, 0.0f, 0.0f), rotation));
	up = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 1.0f, 0.0f), rotation));
}
