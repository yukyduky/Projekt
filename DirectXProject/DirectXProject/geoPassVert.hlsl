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
	float4 positionP	: SV_POSITION;
	float3 positionW	: POSITIONW;
	float4 color		: COLOR;
	float3 normalW		: NORMALW;
	float2 texCoord		: TEXCOORD;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	
	// Convert the position and the normals into world space
	output.positionW = mul(float4(input.position, 1.0f), world).xyz;
	output.normalW = normalize(mul(input.normal, (float3x3)world));
	// Convert the position into projection space
	output.positionP = mul(float4(input.position, 1.0f), wvp);

	// Convert the x-tangent to world space
	//output.tangent1W = normalize(mul(input.tangent.xyz, (float3x3)world));
	// Create the y tangent ---------------------------------------------
	//output.tangent2W = normalize(cross(output.normalW, output.tangent1W)) * input.tangent.w;

	// Do nothing with the texture coords
	output.texCoord = input.texCoord;
	output.color = input.color;

	return output;
}