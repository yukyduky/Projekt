struct Light
{
	float3 pos;
	float range;
	float3 dir;
	float spotlightAngle;
	float4 diffuse;
	float3 ambient;
	float pad1;
	float3 attenuation;
	float pad2;
};

cbuffer cbLightLighting
{
	Light light;
	float3 cameraPos;
	float pad;
};

Texture2D texNormal		: register(t0);
Texture2D texDiffuse	: register(t1);
Texture2D texSpecular	: register(t2);
Texture2D texPosition	: register(t3);

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular, out float specularPower);
float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float specularPower);

float4 PS(float4 position_S : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 diffuse;
	float3 pos;
	float3 specular;
	float specularPower;

	// Load the data from all the textures
	LoadGeoPassData(position_S.xy, normal, diffuse, pos, specular, specularPower);

	// Calculate lightning
	float4 lighting = CalcLight(normal, diffuse, pos, specular, specularPower);

	return lighting;
}

void LoadGeoPassData(in float2 screenCoords, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular, out float specularPower)
{
	int3 texCoords = int3(screenCoords, 0);

	normal = texNormal.Load(texCoords).xyz;
	diffuse = texDiffuse.Load(texCoords).xyz;
	pos = texPosition.Load(texCoords).xyz;
	float4 spec = texSpecular.Load(texCoords);

	specular = spec.xyz;
	specularPower = spec.w;
}

float4 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float specularPower)
{
	// Vector from object to light
	float3 pToL = light.pos - pos;
	// Length of the vector is the distance between the object to the light
	float distance = length(pToL);

	// Create and ambient color from the diffuse color of the object
	float3 finalAmbient = diffuse * light.ambient;

	// Calculate the attenuation depending on how far away the light is
	//float attenuation = max(0, light.attenuation - (distance / light.range));
	/* Directional light will have an attenuation of -1 to indicate infinite range -> 1.0f - (distance / light.attenuation = 2.0f */
	//attenuation = min(1.0f, light.attenuation - (distance / light.range));

	// Normalize pToL
	pToL /= distance;

	// Calculate the "angle" between the normal and the light vector
	float lightIntensity = saturate(dot(normal, pToL));
	// Calculate the diffuse against the light and multiply with the light diffuse color and the "strength" of the light
	float3 finalColor = lightIntensity * diffuse * light.diffuse;
	// Calculate the light falloff factor, aka attenuation
	finalColor /= light.attenuation[0] + light.attenuation[1] * distance + light.attenuation[2] * distance * distance;

	// Add the ambient and saturate to clamp between 0 and 1
	float3 lighting = saturate(finalColor + finalAmbient);

	return float4(lighting, 1.0f);
}