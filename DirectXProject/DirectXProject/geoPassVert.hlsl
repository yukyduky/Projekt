cbuffer cbGeoObject
{
	float4x4 world;
	float4x4 wvp;
};

struct VS_IN
{
	float4 position		: POSITION;
	float2 texCoord		: TEXCOORD;
	float3 normal		: NORMAL;
};

struct VS_OUT
{
	float4 positionP	: SV_POSITION;
	float3 positionW	: POSITIONW;
	float2 texCoord		: TEXCOORD;
	float3 normalW		: NORMALW;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	
	// Convert the position and the normals into world space
	output.positionW = mul(input.position, world).xyz;
	output.normalW = normalize(mul(input.normal, (float3x3)world));
	// Convert the position into projection space
	output.positionP = mul(input.position, wvp);

	// Convert the x-tangent to world space
	//output.tangent1W = normalize(mul(input.tangent.xyz, (float3x3)world));
	// Create the y tangent ---------------------------------------------
	//output.tangent2W = normalize(cross(output.normalW, output.tangent1W)) * input.tangent.w;

	// Do nothing with the texture coords
	output.texCoord = input.texCoord;

	return output;
}