struct VS_IN
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
}

struct VS_OUT
{

}

VS_OUT VS(VS_IN input)
{
	VS_OUT output = input;


	return output;
}