Texture2D HorizontalBlurMap : register(t7);
SamplerState BlurSampler : register (s0);

struct PS_IN
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEXCOORD0;
};

float4 PS(PS_IN input) : SV_TARGET
{
	//The final color which will be our output.
	float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//Max value of color used later for easier readablility of the code.
	float colDiv = 255.0f;

	//Calculate texelSize depending on the width/height of the image.
	float texelSize = 1.0f / 500.0f;

	//Choose how many neighboring pixels to blur from
	const float nrOfTexels = 30;

	//Arrays that will store the texCoords of 10 neighboring texels above and bellow the current.
	float2 vTexCoordDown[nrOfTexels + 1];
	float2 vTexCoordUp[nrOfTexels];
	int weights[nrOfTexels + 1];

	//Sample the current texel we are looking at from the GlowMap.
	float4 currentPixel = HorizontalBlurMap.Sample(BlurSampler, input.texCoord).rgba;
	
	//Variable to keep track of the sum of all the weights for future calculations.
	float weightSum = 0.0f;

	//Calculate weights
	for (int i = 0; i < nrOfTexels + 1; i++)
	{
		weights[nrOfTexels - i] = i + 1;
	}

	//Calculate the texCoords of the neighboring texels
	for (int i = 0; i < nrOfTexels + 1; i++)
	{
		vTexCoordDown[i] = float2( input.texCoord.x, input.texCoord.y + (texelSize * i ));
	}

	for (int i = 0; i < nrOfTexels; i++)
	{
		vTexCoordUp[i] = float2(input.texCoord.x, input.texCoord.y - (texelSize * (i + 1)));
	}


	//Blur the output texel with the neigboring texels in the texelarray we created earlier
	for (int i = 0; i < nrOfTexels; i++)
	{
		output.xyz += HorizontalBlurMap.Sample(BlurSampler, vTexCoordDown[i]) * weights[i];
		weightSum += weights[i];
		output.xyz += HorizontalBlurMap.Sample(BlurSampler, vTexCoordUp[i]) * weights[i + 1];
		weightSum += weights[i + 1];
	}

	//Divide color with the sum of all the used weights
	output.xyz /= (weightSum/2);
	weightSum = 0.0f;
	
	output.w = 1.0f;

	return output;
}
