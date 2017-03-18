struct VS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;
};

struct VS_OUT
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	
	output.position = input.position;
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	output.tangent = input.tangent;

	return output;
}