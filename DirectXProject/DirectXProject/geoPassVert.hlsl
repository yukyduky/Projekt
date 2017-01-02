cbuffer cbGeoObject		: register(b0)
{
	float4x4 world;
	float4x4 wvp;
};

struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

struct VS_OUT
{
	float4 position_P	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float4 color		: COLOR;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float3 tangent_W		: TANGENTW;
	float3 bitangent_W	: BITANGENTW;
	//float3x3 TBN_W		: TANGENTMATRIXW;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	
	// Convert the position and the normal into world space
	output.position_W = mul(float4(input.position, 1.0f), world).xyz;
	output.normal_W = normalize(mul(input.normal, (float3x3)world));
	// Convert the position into projection space
	output.position_P = mul(float4(input.position, 1.0f), wvp);

	// Convert the x-tangent to world space
	output.tangent_W = normalize(mul(float3(1.0f, 0.0f, 0.0f), (float3x3)world));
	// Create the y tangent ---------------------------------------------
	output.bitangent_W = normalize(cross(output.tangent_W, output.normal_W)); // * input.tangent.w;

	// Do nothing with the texture coords
	output.texCoord = input.texCoord;
	output.color = input.color;

	return output;
}