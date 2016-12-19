struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

float4 PS(PS_IN input) : SV_TARGET
{
	float4 finalColor = input.color;

	return finalColor;
}