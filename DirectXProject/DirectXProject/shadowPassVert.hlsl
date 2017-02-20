cbuffer cbShadowObject	: register(c0)
{
	float4x4 wvp;
};

float4 VS(float3 position : POSITION) : SV_POSITION
{	
	return mul(float4(position, 1.0f), wvp);
}