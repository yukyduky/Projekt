struct PSIN
{

};

float4 PS(PSIN input) : SV_TARGET
{
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return finalColor;
}