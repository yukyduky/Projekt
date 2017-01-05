struct PointLight
{
	float3 pos;
	float pad0;
	float4 diffuse;
	float3 ambient;
	float pad1;
	float3 attenuation;
	float specPower;
};

struct SpotLight
{
	float3 pos;
	float pad0;
	float3 dir;
	float angle;
	float4 diffuse;
	float3 ambient;
	float pad1;
	float3 attenuation;
	float specPower;
};

struct DirectLight
{
	float3 pos;
	float pad0;
	float3 dir;
	float pad1;
	float4 diffuse;
	float3 ambient;
	float specPower;
};

struct GeneralLightAttrb
{
	float3 cameraDir;
	float pad;
};

cbuffer cbLightLighting
{
	PointLight pointLight;
	SpotLight spotLight;
	DirectLight directLight;
	GeneralLightAttrb genLight;
};

static const int NUM_POINTLIGHTS = 0;
static const int NUM_SPOTLIGHTS = 0;
static const int NUM_DIRECTLIGHTS = 1;

Texture2D texNormal		: register(t0);
Texture2D texDiffuse	: register(t1);
Texture2D texSpecular	: register(t2);
Texture2D texPosition	: register(t3);

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular);
float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular);

float4 PS(float4 position_S : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 diffuse;
	float3 pos;
	float3 specular;

	// Load the data from all the textures
	LoadGeoPassData(position_S.xy, normal, diffuse, pos, specular);

	// Calculate lightning
	float4 lighting = CalcLight(normal, diffuse, pos, specular);

	return lighting;
}

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular)
{
	int3 texCoords = int3(screenCoords, 0);

	normal = texNormal.Load(texCoords).xyz;
	diffuse = texDiffuse.Load(texCoords).xyz;
	pos = texPosition.Load(texCoords).xyz;
	specular = texSpecular.Load(texCoords).xyz;
}

float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular)
{
	float3 pointLighting = float3(0.0f, 0.0f, 0.0f);
	float3 spotLighting = float3(0.0f, 0.0f, 0.0f);
	float3 directLighting = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < NUM_POINTLIGHTS; i++)
	{
		// Vector from object to light
		float3 pToL = pointLight.pos - pos;
		// Length of the vector is the distance between the object to the light
		float distance = length(pToL);
		// Normalize pToL
		pToL /= distance;

		// Calculate the "angle" between the normal and the light vector
		float lightIntensity = saturate(dot(normal, pToL));
		// Calculate the diffuse against the light and multiply with the light diffuse color
		float3 finalColor = lightIntensity * diffuse * pointLight.diffuse.xyz;
		// Calculate the light falloff factor, aka attenuation
		finalColor /= pointLight.attenuation[0] + pointLight.attenuation[1] * distance + pointLight.attenuation[2] * distance * distance;
		
		// Create an ambient color from the diffuse color of the object
		float3 finalAmbient = diffuse * pointLight.ambient;

		// Reflection vector based on the lightIntensity, normal, and the light direction
		float3 reflect = normalize(2 * lightIntensity * normal - pToL);
		// Calculate the amount of light reflected
		float3 spec = pow(saturate(dot(reflect, -genLight.cameraDir)), pointLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular
		pointLighting = finalColor + finalAmbient + spec;
	}
	for (int i = 0; i < NUM_SPOTLIGHTS; i++)
	{
		// Vector from object to light
		float3 pToL = spotLight.pos - pos;
		// Length of the vector is the distance between the object to the light
		float distance = length(pToL);
		// Normalize pToL
		pToL /= distance;

		// Calculate the "angle" between the normal and the light vector
		float lightIntensity = saturate(dot(normal, pToL));
		// Calculate the diffuse against the light and multiply with the light diffuse color
		float3 finalColor = lightIntensity * diffuse * spotLight.diffuse.xyz;
		// Calculate the light falloff factor, aka attenuation
		finalColor /= spotLight.attenuation[0] + spotLight.attenuation[1] * distance + spotLight.attenuation[2] * distance * distance;
		// Calculate the attenuation from the center of the cone to the edge of the spotlight
		finalColor *= pow(saturate(dot(-pToL, spotLight.dir)), spotLight.angle);

		// Create an ambient color from the diffuse color of the object
		float3 finalAmbient = diffuse * spotLight.ambient;

		// Reflection vector based on the lightIntensity, normal, and the light direction
		float3 reflect = normalize(2 * lightIntensity * normal - pToL);
		// Calculate the amount of light reflected
		float3 spec = pow(saturate(dot(reflect, -genLight.cameraDir)), spotLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular
		spotLighting = finalColor + finalAmbient + spec;
	}
	for (int i = 0; i < NUM_DIRECTLIGHTS; i++)
	{
		// Calculate the "angle" between the normal and the light direction
		float lightIntensity = saturate(dot(normal, -directLight.dir));
		// Calculate the diffuse against the light and multiply with the light diffuse color
		float3 finalColor = lightIntensity * diffuse * directLight.diffuse.xyz;

		// Create an ambient color from the diffuse color of the object
		float3 finalAmbient = diffuse * directLight.ambient;

		// Reflection vector based on the lightIntensity, normal, and the light direction
		float3 reflect = normalize(2 * lightIntensity * normal + directLight.dir);
		// Calculate the amount of light reflected
		float3 spec = pow(saturate(dot(reflect, -genLight.cameraDir)), directLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular
		directLighting = finalColor + finalAmbient + spec;
	}
	
	// Combine all the different lightsources and clamp it
	float3 lighting = saturate(pointLighting + spotLighting + directLighting);

	return float4(lighting, 1.0f);
}