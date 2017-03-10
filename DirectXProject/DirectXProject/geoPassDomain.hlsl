
Texture2D mapDisplace		: register(t1);
SamplerState AnisoSampler	: register(s0);

cbuffer cbGeoObject			: register(b0)
{
	float4x4 world;
	float4x4 wvp;
};

cbuffer cbTess				: register(b1)
{
	float tessStart;
	float3 camPos;
};

struct Const_OUT
{
	float edges[3]	: SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct DS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;

};


struct DS_OUT
{
	float4 position_P	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float4 tangent_W	: TANGENTW;

};

[domain("tri")]
DS_OUT DS(Const_OUT input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<DS_IN, 3> patch)
{
	DS_OUT output;

	// Set the new position with the patch from the HullShader
	float3 vertexPos = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
	// Set the new varibles so it can be used in the displacement
	float4 newTangent = uvwCoord.x * patch[0].tangent + uvwCoord.y * patch[1].tangent + uvwCoord.z * patch[2].tangent;
	float3 newNormal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;
	float2 newTex = uvwCoord.x * patch[0].texCoord + uvwCoord.y * patch[1].texCoord + uvwCoord.z * patch[2].texCoord;

	//////////DISPLACEMENT//////////////////
	// Only triangles in the boxes will have a tessStart of 3.0f
	if (tessStart == 3.0f)
	{
		// loads the distance the appied distance from the displacement map
		float displacement = (mapDisplace.SampleLevel(AnisoSampler, newTex, 0).b);// *0.2;

		//moves the position in the direction of the normal
		vertexPos += newNormal * displacement;
	}
	////////////////////////////

	// Convert the position into projection space
	output.position_P = mul(float4(vertexPos, 1.0f), wvp);

	// Convert the position, normal, and the x-tangent into world space
	output.position_W = mul(float4(vertexPos, 1.0f), world).xyz;
	output.tangent_W.xyz = mul(newTangent.xyz, (float3x3)world);

	// The texture coordinate
	output.texCoord = newTex;

	return output;
}