#include "Camera.h"


Camera::Camera() : up(Vector3(0.0f, 1.0f, 0.0f)), forward(Vector3(0.0f, 0.0f, 1.0f)), right(Vector3(1.0f, 0.0f, 0.0f)),
					nearD(1.0f), farD(1000.0f), fov(72), ar(float(WIDTH / HEIGHT))
{
	this->worldUp = Vector3(0.0f, 1.0f, 0.0f);
	this->worldForward = Vector3(0.0f, 0.0f, 1.0f);
	this->worldRight = Vector3(1.0f, 0.0f, 0.0f);
	// Position
	this->pos = Vector3(2.0f, 40.0f, 2.0f);

	// Set the view matrix
	this->view = XMMatrixLookAtLH(pos, forward, up);
	// Set the projection matrix
	proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(this->fov), this->ar, this->nearD, this->farD);

	this->yaw = 0.0f;
	this->pitch = 0.0f;

	this->flyMode = 1;
	this->moveLR = 0.0f;
	this->moveBF = 0.0f;
}

Camera::~Camera()
{
}

void Camera::InitScene()
{

}

void Camera::Update(InputVars inVars, float dt, float* heightValues)
{
	if (this->flyMode == 0)
	{
		this->ProcessKeyboard(inVars, dt);
		this->ProcessMouse(inVars.mouseOffset, dt);

		this->rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(this->pitch), XMConvertToRadians(this->yaw), 0);

		this->forward = XMVector3TransformCoord(this->worldForward, this->rotationMatrix);
		this->forward.Normalize();

		Matrix rotateYTempMatrix;
		rotateYTempMatrix = XMMatrixRotationY(XMConvertToRadians(this->yaw));

		this->right = XMVector3TransformCoord(this->worldRight, rotateYTempMatrix);
		this->right.Normalize();
		this->up = XMVector3TransformCoord(this->worldUp, rotateYTempMatrix);
		this->up.Normalize();
		this->target = XMVector3TransformCoord(this->worldForward, rotateYTempMatrix);
		this->target.Normalize();

		this->pos += this->moveLR * this->right;
		this->pos += this->moveBF * this->target;
		
		//////////////////////////////////////////////////////////
		int index = (int)this->pos.x + (int)this->pos.z * 256;
		this->pos.y = (heightValues[index] / 10.0f) + 5.0f;
		
		//////////////////////////////////////////////////////////

		this->moveLR = 0.0f;
		this->moveBF = 0.0f;

		this->view = XMMatrixLookAtLH(this->pos, this->pos + this->forward, this->up);
		
	}
	else if (this->flyMode == 1)
	{
		this->ProcessKeyboard(inVars, dt);
		this->ProcessMouse(inVars.mouseOffset, dt);

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

Vector3 Camera::getRight() const
{
	return this->right;
}

float Camera::getNearDist() const
{
	return this->nearD;
}

float Camera::getFarDist() const
{
	return this->farD;
}

float Camera::getFOV() const
{
	return this->fov;
}

float Camera::getAR() const
{
	return this->ar;
}

void Camera::ProcessKeyboard(InputVars inVars, float dt)
{
	float velocity = 0;

	if (this->flyMode == 0)
	{
		velocity = 100.0f * dt;

		if (inVars.isKeyPressed[W])
			this->moveBF += velocity;
		if (inVars.isKeyPressed[A])
			this->moveLR -= velocity;
		if (inVars.isKeyPressed[S])
			this->moveBF -= velocity;
		if (inVars.isKeyPressed[D])
			this->moveLR += velocity;
		if (inVars.wasKeyPressed[F])
		{
			this->flyMode = 1;
		}
	}
	else if (this->flyMode == 1)
	{
		velocity = 100.0f * dt;

		// Change the position according to the key pressed
		if (inVars.isKeyPressed[W])
			this->pos += this->forward * velocity;
		if (inVars.isKeyPressed[A])
			this->pos -= this->right * velocity;
		if (inVars.isKeyPressed[S])
			this->pos -= this->forward * velocity;
		if (inVars.isKeyPressed[D])
			this->pos += this->right * velocity;
		if (inVars.wasKeyPressed[F])
		{
			this->flyMode = 0;
		}
	}
}

void Camera::ProcessMouse(Vector2 mouseOffset, float dt)
{
	float sensitivity = -100.0f;

	// Change the yaw and pitch depending on the mouse offset since the last frame
	yaw += mouseOffset.x * sensitivity * dt;
	pitch += mouseOffset.y * sensitivity * dt;

	// Makes sure the camera doesn't get flipped over
	if (this->pitch > 80.0f)
		this->pitch = 80.0f;
	if (this->pitch < -80.0f)
		this->pitch = -80.0f;

	if (this->flyMode == 1)
	{
		// Create the rotation matrix
		Matrix rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), 0.0f);

		// Recalculate the vectors
		forward = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 0.0f, 1.0f), rotation));
		right = XMVector3Normalize(XMVector3TransformCoord(Vector3(1.0f, 0.0f, 0.0f), rotation));
		up = XMVector3Normalize(XMVector3TransformCoord(Vector3(0.0f, 1.0f, 0.0f), rotation));

		this->target = this->forward;
	}
}
