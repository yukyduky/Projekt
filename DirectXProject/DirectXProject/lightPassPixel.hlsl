struct Light
{
	float3 pos;
	float attenuation;
	float3 dir;
	float spotlightAngle;
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
	float3 pToL = light.pos - pos;
	float distance = length(pToL);

	// Calculate the attenuation depending on how far away the light is
	float attenuation = max(0, 1.0f - (distance / light.attenuation));
	// Directional light will have an attenuation of -1 to indicate infinite range -> 1.0f - (distance / light.attenuation = 2.0f
	attenuation = min(1.0f, 1.0f - (distance / light.attenuation));

	// Normalize pToL
	pToL /= distance;

	// Calculate the "angle" between the normal and the light vector
	float lightIntensity = saturate(dot(normal, pToL));
	// Calculate the diffuse against the light
	float3 finalDiffuse = lightIntensity * diffuse;

	// Combine everything and return it
	float3 lighting = finalDiffuse * attenuation;

	return float4(lighting, 1.0f);
}