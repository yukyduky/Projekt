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



float4 PS(float4 positionS : SV_POSITION) : SV_TARGET0
{
	//float4 normal = 


	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}