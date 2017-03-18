Texture2D DiffuseGlowMap : register(t7);
SamplerState BlurSampler : register (s0);

struct PS_IN
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEXCOORD0;
};


float4 PS(PS_IN input) : SV_TARGET
{
	//The final color which will be our output
	float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//Max value of color used later for easier readability of the code
	float colorDiv = 255.0f;

	//Calculate texelSize depending on the width of the image
	float texelSize = 1.0f / 500.0f;

	const float nrOfTexels = 30;

	//Array with texCoords of the 10 neighboring texels to the left and right of current texel
	float2 hTexCoordLeft[nrOfTexels + 1];
	float2 hTexCoordRight[nrOfTexels];

	//Weights that determine how much color should be blurred into the texel depending on distance from current texel
	int weights[nrOfTexels + 1];

	//Sample the current texel we are looking at from the DiffuseGlowMap
	float4 currentPixel = DiffuseGlowMap.Sample(BlurSampler, input.texCoord).rgba;

	//Variable to keep track of the sum of all the used weights for future calculations
	float weightSum = 0.0f;

	for (int i = 0; i < nrOfTexels + 1; i++)
	{
		weights[nrOfTexels - i] = i + 1;
	}

	//Calculate the texCoords of the neighboring texels
	for (int i = 0; i < nrOfTexels; i++)
	{
		hTexCoordRight[i] = float2(input.texCoord.x + (texelSize * i), input.texCoord.y);
	}

	for (int i = 0; i < nrOfTexels; i++)
	{
		hTexCoordLeft[i] = float2(input.texCoord.x - (texelSize * (i + 1)), input.texCoord.y);
	}

	//Blur the output texel with the neighboring texels in the texelarray we created earlier
	for (int i = 0; i < nrOfTexels; i++)
	{
		output.xyz += DiffuseGlowMap.Sample(BlurSampler, hTexCoordLeft[i]) * weights[i];
		output.xyz += DiffuseGlowMap.Sample(BlurSampler, hTexCoordRight[i]) * weights[i + 1];
		weightSum += weights[i];
	}

	//Divide color with the sum of all the used weights
	output.xyz /= weightSum;
	
	//Set w to 1
	output.w = 1.0f;

	return output;
}
