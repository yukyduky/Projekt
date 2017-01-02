struct Light
{
	float3 pos;
	float pad1;
	float3 color;
	float pad2;
	float3 dir;
	float pad3;
	float4 range;
	float2 spotlightAngles;
	float2 pad4;
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

void LoadGeoPassData(in float2 screenPos, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular, out float specularPower);
float3 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float specularPower);

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
	float3 lighting = CalcLight(normal, diffuse, pos, specular, specularPower);

	return float4(lighting, 1.0f);
}

void LoadGeoPassData(in float2 screenPos, out float3 normal, out float3 diffuse, out float3 pos, out float3 specular, out float specularPower)
{
	int3 texCoords = int3(screenPos, 0);

	normal = texNormal.Load(texCoords).xyz;
	diffuse = texDiffuse.Load(texCoords).xyz;
	pos = texPosition.Load(texCoords).xyz;
	float4 spec = texSpecular.Load(texCoords);

	specular = spec.xyz;
	specularPower = spec.w;
}

float3 CalcLight(in float3 normal, in float3 diffuse, in float3 pos, in float3 specular, in float specularPower)
{
	return diffuse;
}