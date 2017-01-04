#include "Light.h"

Light::Light(int lightType, Vector3 lightPosition) 
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
}

Light::Light(int lightType, Vector3 lightPosition, Vector3 lightDirection, float attenuation)
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
	this->lightDirection = lightDirection;
	this->attenuation = attenuation;
}

Light::Light(int lightType, Vector3 lightPosition, Vector3 lightDirection, float attenuation, float angle)
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
	this->lightDirection = lightDirection;
	this->attenuation = attenuation;
	this->angle = angle;
}

Light::~Light() 
{

}

int Light::getLightType() 
{
	return this->lightType;
}
