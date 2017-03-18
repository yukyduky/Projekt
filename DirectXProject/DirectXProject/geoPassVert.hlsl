cbuffer cbGeoObject		: register(b0)
{
	float4x4 world;
	float4x4 wvp;
};

struct VS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;
};

struct VS_OUT
{
	float4 position_P	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float4 tangent_W	: TANGENTW;

	
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	
	// Convert the position into projection space
	output.position_P = mul(float4(input.position, 1.0f), wvp);
	// Convert the position, normal, and the x-tangent into world space
	output.position_W = mul(float4(input.position, 1.0f), world).xyz;
	output.normal_W = mul(input.normal, (float3x3)world);
	output.tangent_W.xyz = mul(input.tangent.xyz, (float3x3)world);

	// Do nothing with the texture coords
	output.texCoord = input.texCoord;

	return output;
}