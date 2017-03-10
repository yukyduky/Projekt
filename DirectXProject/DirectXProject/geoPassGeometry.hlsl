cbuffer cbTess				: register(b1)
{
	float tessStart;
	float3 camPos;
};

struct GS_IN
{
	float4 position_P	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float4 tangent_W	: TANGENTW;
};

struct GS_OUT
{
	float4 position_P	: SV_POSITION;
	float3 position_W	: POSITIONW;
	float3 normal_W		: NORMALW;
	float2 texCoord		: TEXCOORD;
	float4 tangent_W	: TANGENTW;
};

[maxvertexcount(3)]
void GS(triangle GS_IN input[3], inout TriangleStream<GS_OUT> Tstream)
{
	GS_OUT output;

	//Calculates the normal of the triangle
	float3 side1 = (input[1].position_W.xyz - input[0].position_W.xyz),
		side2 = (input[2].position_W.xyz - input[0].position_W.xyz);
	float3 normal = cross(side1.xyz, side2.xyz);
	normalize(normal);

	for (int i = 0; i < 3; i++)
	{
		output = input[i];

		output.normal_W = normal;

		// if the normal if facing away from the camera the triangle wont be appended
		if(dot(output.normal_W, (output.position_W - camPos)) < 0.0f)
			Tstream.Append(output);

	}
}