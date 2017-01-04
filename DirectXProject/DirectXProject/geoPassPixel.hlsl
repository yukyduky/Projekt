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
	float4 position_S	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float4 tangent_W	: TANGENTW;
};

struct PS_OUT
{
	float4 normal			: SV_TARGET0;
	float4 diffuse			: SV_TARGET1;
	float4 specular			: SV_TARGET2;
	float4 position			: SV_TARGET3;
};

PS_OUT PS(PS_IN input)
{
	PS_OUT output;

	// Sample the texture for the diffuse light
	float3 diffuse = DiffuseMap.Sample(AnisoSampler, input.texCoord).rgb;
	// Sample the normal map in tangent space and decompress
	float3 normal_T = NormalMap.Sample(AnisoSampler, input.texCoord).rgb;
	normal_T = (normal_T * 2.0f) - 1.0f;

	// Create the bitangent
	float3 bitangent_W = cross(input.normal_W, input.tangent_W.xyz) * input.tangent_W.w;
	// Create the tangent matrix
	float3x3 TBN_W = float3x3(normalize(input.tangent_W.xyz), normalize(bitangent_W), normalize(input.normal_W));

	// Convert the normal to world space
	float3 normal_W = normalize(mul(normal_T, TBN_W));

	output.normal = float4(normal_W, 1.0f);
	output.diffuse = float4(diffuse, 1.0f);
	output.specular = float4(specular, specularPower);
	output.position = float4(input.position_W, 1.0f);

	return output;
}