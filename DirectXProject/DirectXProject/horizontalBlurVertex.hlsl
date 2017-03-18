struct VS_IN
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};


struct VS_OUT
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEXCOORD0;
};


VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.position = input.position;
	output.texCoord = input.texCoord;

	return output;
}