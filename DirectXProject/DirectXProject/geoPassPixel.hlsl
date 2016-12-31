cbuffer cbGeoLighting
{
	float3 specular;
	float specularPower;
	float3x3 tangentW;
};

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState AnisoSampler : register(s0);

struct PS_IN
{
	float4 positionS	: SV_POSITION;
	float3 positionW	: POSITIONW;
	float4 color		: COLOR;
	float3 normalW		: NORMALW;
	float2 texCoord		: TEXCOORD;
};

struct PS_OUT
{
	/*float4 normal		: SV_TARGET0;
	float4 diffuse		: SV_TARGET1;
	float4 specular		: SV_TARGET2;
	float4 position		: SV_TARGET3;*/
};

float4 PS(PS_IN input) : SV_TARGET
{
	//PS_OUT output;
	float4 color = input.color;

	/*// Sample the texture for the diffuse light
	float3 diffuse = DiffuseMap.Sample(AnisoSampler, input.texCoord).rgb;

	// Normalize the tangent matrix after interpolation ------------------------------------------------------
	//tangentW = float3x3(normalize(input.tangent1W), normalize(input.tangent2W), normalize(input.normalW));
	//tangentW = normalize(tangentW);
	
	// Sample the normal map in tangent space and decompress -------------------------------------------------
	float3 normalT = NormalMap.Sample(AnisoSampler, input.texCoord).rgb;
	normalT = normalize(normalT * 2.0f - 1.0f);

	// Convert the normal to world space
	float3 normalW = mul(normalT, tangentW);*/

	/*output.normal = float4(normalW, 1.0f);
	output.diffuse = float4(diffuse, 1.0f);
	output.specular = float4(specular, specularPower);
	output.position = float4(input.positionW, 1.0f);*/

	return color;
}