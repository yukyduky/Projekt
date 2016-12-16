struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.pos = float4(input.pos, 1);
	output.color = input.color;
	output.texCoord = input.texCoord;

	return output;
}