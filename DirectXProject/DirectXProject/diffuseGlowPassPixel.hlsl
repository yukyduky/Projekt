Texture2D DiffuseMap : register(t0);
Texture2D GlowMap : register(t1);
SamplerState BlurSampler : register (s0);

struct PS_IN
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEXCOORD0;
};


float4 PS(PS_IN input) : SV_TARGET
{
	float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);

	
	float3 currentPixel = GlowMap.Sample(BlurSampler, input.texCoord).rgb;
	float colorDiv = 255.0f;


	if (currentPixel.x > 50.0f / colorDiv)
	{
		output.xyz = DiffuseMap.Sample(BlurSampler, input.texCoord).rgb;
		output.w = 1.0f;
	}
	else
	{
		output.xyz = GlowMap.Sample(BlurSampler, input.texCoord).rgb;
		output.w = 1.0f;
	}
	
	return output;
}
