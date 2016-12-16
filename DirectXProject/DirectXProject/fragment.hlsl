struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

float4 PS(PS_IN input) : SV_TARGET
{
	float4 finalColor = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return finalColor;
}