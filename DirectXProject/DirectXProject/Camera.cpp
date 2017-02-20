#include "Camera.h"


Camera::Camera()
{
	// For orientation purposes it needs to know what way is up, forward, and right
	this->up = Vector3(0.0f, 1.0f, 0.0f);
	this->forward = Vector3(0.0f, 0.0f, 1.0f);
	this->right = Vector3(1.0f, 0.0f, 0.0f);
	this->worldForward = Vector3(0.0f, 0.0f, 1.0f);
	this->worldRight = Vector3(1.0f, 0.0f, 0.0f);
	// Position
	this->pos = Vector3(50.0f, 50.0f, 50.0f);

	// Set the view matrix
	this->view = XMMatrixLookAtLH(pos, forward, up);
	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(0.4f * XM_PI, (float)WIDTH / HEIGHT, 0.01f, 1000.0f);


	this->yaw = 0.0f;
	this->pitch = 0.0f;

	this->flyMode = 0;
	this->moveLR = 0.0f;
	this->moveBF = 0.0f;
}

Camera::~Camera()
{
}

void Camera::InitScene()
{

}

void Camera::Update(bool* keys, Vector2 mouseOffset, float dt, float* heightValues)
{
	if (this->flyMode == 0)
	{
		this->ProcessKeyboard(keys, dt);
		this->ProcessMouse(mouseOffset, dt);

		this->rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(this->pitch), XMConvertToRadians(this->yaw), 0);

		this->target = XMVector3TransformCoord(this->worldForward, this->rotationMatrix);
		this->target.Normalize();

		Matrix rotateYTempMatrix;
		rotateYTempMatrix = XMMatrixRotationY(XMConvertToRadians(this->yaw));

		this->right = XMVector3TransformCoord(this->worldRight, rotateYTempMatrix);
		this->right.Normalize();
		this->up = XMVector3TransformCoord(this->up, rotateYTempMatrix);
		this->up.Normalize();
		this->forward = XMVector3TransformCoord(this->worldForward, rotateYTempMatrix);
		this->forward.Normalize();

		this->pos += this->moveLR * this->right;
		this->pos += this->moveBF * this->forward;
		
		//////////////////////////////////////////////////////////
		int index = this->pos.x + (257 - this->pos.z) * 257;
		
		this->pos.y = (heightValues[index] / 10.0f) + 20.0f;
		//////////////////////////////////////////////////////////

		this->moveLR = 0.0f;
		this->moveBF = 0.0f;

		this->target = this->pos + this->target;

		this->view = XMMatrixLookAtLH(this->pos, this->target, this->up);
	}

	else if (this->flyMode == 1)
	{
		this->ProcessKeyboard(keys, dt);
		this->ProcessMouse(mouseOffset, dt);

		// Update the view matrix
		this->view = XMMatrixLookAtLH(this->pos, this->pos + this->forward, this->up);
	}
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

void Camera::ProcessKeyboard(bool* keys, float dt)
{
	float velocity = 0;

	

	if (this->flyMode == 0)
	{
		velocity = 15.0f * dt;

		if (keys[W])
			this->moveBF += velocity;
		if (keys[A])
			this->moveLR -= velocity;
		if (keys[S])
			this->moveBF -= velocity;
		if (keys[D])
			this->moveLR += velocity;
		if (keys[F])
			this->flyMode = 1;

		
	}

	else if (this->flyMode == 1)
	{
		velocity = 100.0f * dt;

		// Change the position according to the key pressed
		if (keys[W])
			this->pos += this->forward * velocity;
		if (keys[A])
			this->pos -= this->right * velocity;
		if (keys[S])
			this->pos -= this->forward * velocity;
		if (keys[D])
			this->pos += this->right * velocity;
		if (keys[F])
			this->flyMode = 0;

	}
		
	

}

void Camera::ProcessMouse(Vector2 mouseOffset, float dt)
{
	float sensitivity = -1000.0f;

	// Change the yaw and pitch depending on the mouse offset since the last frame
	yaw += mouseOffset.x * sensitivity * dt;
	pitch += mouseOffset.y * sensitivity * dt;

	// Makes sure the camera doesn't get flipped over
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	if (this->flyMode == 1)
	{
		// Create the rotation matrix
		Matrix rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), 0.0f);

		// Recalculate the vectors
		forward = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 0.0f, 1.0f), rotation));
		right = XMVector3Normalize(XMVector3TransformCoord(Vector3(1.0f, 0.0f, 0.0f), rotation));
		up = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 1.0f, 0.0f), rotation));
	}
}
