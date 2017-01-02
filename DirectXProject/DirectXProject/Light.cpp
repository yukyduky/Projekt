#include "Light.h"

Light::Light(int lightType, Vector4 lightPosition) 
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
}

Light::Light(int lightType, Vector4 lightPosition, Vector4 lightDirection)
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
	this->lightDirection = lightDirection;
}

Light::Light(int lightType, Vector4 lightPosition, Vector4 lightDirection, float attenuation)
{
	this->lightType = lightType;
	this->lightPosition = lightPosition;
	this->lightDirection = lightDirection;
	this->attenuation = attenuation;
}

Light::~Light() 
{

}

int Light::getLightType() 
{
	return this->lightType;
}
