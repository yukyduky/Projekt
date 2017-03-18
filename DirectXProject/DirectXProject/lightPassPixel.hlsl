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
	float4x4 shadowMatrix;
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
	float3 cameraPos;
	float pad;
};

cbuffer cbLightLighting	: register(b0)
{
	PointLight pointLight;
	SpotLight spotLight;
	DirectLight directLight;
	GeneralLightAttrb genLight;
};

SamplerState ShadowSampler	: register(s0);

static const int NUM_POINTLIGHTS = 0;
static const int NUM_SPOTLIGHTS = 1;
static const int NUM_DIRECTLIGHTS = 0;

Texture2D texNormal		: register(t0);
Texture2D texDiffuse	: register(t1);
Texture2D texSpecular	: register(t2);
Texture2D texPosition	: register(t3);
Texture2D texShadowMap	: register(t4);

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular);
void SampleShadowMaps(in float2 shadowCoords, out float shadow, in float depth);
void TransformSpaces(in float4 pos_W, out float4 lightSpace);
float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float shadow);

float4 PS(float4 position_S : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 diffuse;
	float3 pos;
	float3 specular;
	float shadow;
	float4 posLight;

	// Load the data from all the textures
	LoadGeoPassData(position_S.xy, normal, diffuse, pos, specular);

	// Transform the world pos into the light's NDC space and decompress for sampling
	TransformSpaces(float4(pos, 1.0f), posLight);

	float2 shadowCoords = posLight.xy;
	float depth = posLight.z;

	// Load the shadowmaps
	SampleShadowMaps(shadowCoords, shadow, depth);

	// Calculate lightning
	float4 lighting = CalcLight(normal, diffuse, pos, specular, shadow);

	return lighting;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular)
{
	int3 texCoords = int3(screenCoords, 0.0f);

	normal = texNormal.Load(texCoords).xyz;
	diffuse = texDiffuse.Load(texCoords).xyz;
	pos = texPosition.Load(texCoords).xyz;
	specular = texSpecular.Load(texCoords).xyz;
}

void SampleShadowMaps(in float2 shadowCoords, out float shadow, in float depth)
{
	// Samples the shadowmap's depth and checks it against the depth of the chosen pixel's position
	// if the shadowmap's depth is less than the pixel's then it's in shadow ( ? true : false )
	// "+ 0.000001" is the bias offset against shadow acne, aka z-fighting
	shadow = texShadowMap.Sample(ShadowSampler, shadowCoords).r < depth ? 0.0f : 1.0f;
}

void TransformSpaces(in float4 pos_W, out float4 lightSpace)
{
	// Transform the position from world to the lights projection space
	lightSpace = mul(pos_W, spotLight.shadowMatrix);

	// Perspective divide into NDC space, needed to scale things depending on how far away/close they are
	lightSpace /= lightSpace.w;
	// Decompresses the values from the range of (-1 <-> 1) to (0 <-> 1), needed because the UV coordinates for texture sampling are in the range of (0 <-> 1)
	lightSpace.x = ((1.0f + lightSpace.x) / 2.0f);
	lightSpace.y = ((1.0f - lightSpace.y) / 2.0f);
}

float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float shadow)
{
	float3 pointLighting = float3(0.0f, 0.0f, 0.0f);
	float3 spotLighting = float3(0.0f, 0.0f, 0.0f);
	float3 directLighting = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < NUM_POINTLIGHTS; i++)
	{
		// Vector from object to light
		float3 pToL = pointLight.pos - pos;
		// Vector from object to camera
		float3 pToC = normalize(genLight.cameraPos - pos);
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
		float3 spec = pow(saturate(dot(reflect, pToC)), pointLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular
		pointLighting = finalColor + finalAmbient + spec;
	}
	for (int i = 0; i < NUM_SPOTLIGHTS; i++)
	{
		// Vector from object to light
		float3 pToL = spotLight.pos - pos;
		// Vector from object to camera
		float3 pToC = normalize(genLight.cameraPos - pos);
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
		float3 spec = pow(saturate(dot(reflect, pToC)), spotLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular, if the pixel is in shadow then shadow = 0.0f, otherwise shadow = 1.0f
		spotLighting = finalColor * shadow + finalAmbient + spec * shadow;
	}
	for (int i = 0; i < NUM_DIRECTLIGHTS; i++)
	{
		// Vector from object to camera
		float3 pToC = normalize(genLight.cameraPos - pos);
		// Calculate the "angle" between the normal and the light direction
		float lightIntensity = saturate(dot(normal, -directLight.dir));
		// Calculate the diffuse against the light and multiply with the light diffuse color
		float3 finalColor = lightIntensity * diffuse * directLight.diffuse.xyz;

		// Create an ambient color from the diffuse color of the object
		float3 finalAmbient = diffuse * directLight.ambient;
		
		float test = normalize(dot(normal, directLight.dir));
		test = max(test, 0);

		// Reflection vector based on the lightIntensity, normal, and the light direction
		float3 reflect = normalize(2 * lightIntensity * normal + directLight.dir);
		// Calculate the amount of light reflected
		float3 spec = pow(saturate(dot(reflect, pToC)), directLight.specPower);
		// Multiply with the amount of specular light at this current pixel
		spec *= specular;

		// Add the ambient and the specular
		directLighting = finalColor + finalAmbient + spec;
	}
	
	// Combine all the different lightsources and clamp it
	float3 lighting = saturate(pointLighting + spotLighting + directLighting);

	return float4(lighting, 1.0f);
}