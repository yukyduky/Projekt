cbuffer cbTess		: register(b1)
{
	float tessStart;
	float3 camPos;
};

struct Const_OUT
{
	float edges[3]	: SV_TessFactor;
	float inside : SV_InsideTessFactor;
};


struct HS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;
};

struct HS_OUT
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float4 tangent		: TANGENT;
};

///CONSTANT FUNKTION///////
Const_OUT PatchConstantFunction(InputPatch<HS_IN, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	Const_OUT output;

	float tessVarible = 1.0f;

	if (tessStart == 3.0f)
	{
		//The tesselation will depend on the side of the triangle;
		tessVarible = length(float3(inputPatch[0].position - inputPatch[1].position));
		float tess2 = length(float3(inputPatch[0].position - inputPatch[2].position));
		if (tess2 < tessVarible)
			tessVarible = tess2;

	}

	// Apply the Tesselation Varible to all the edges;
	output.edges[0] = tessVarible;
	output.edges[1] = tessVarible;
	output.edges[2] = tessVarible;

	// Lastly the inside of the triangle aswell
	output.inside = tessVarible;

	return output;
}
////////////////////////////


///HEADER (attributes)///
[domain("tri")]//Patch type
[partitioning("integer")]//Partition type (intiger will round upp)
[outputtopology("triangle_cw")]//The output type (cw = clockwise)
[outputcontrolpoints(3)]//Pathc size
[patchconstantfunc("PatchConstantFunction")]//Name of the Constant function
HS_OUT HS(InputPatch<HS_IN, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HS_OUT output;

	// Pass on the position
	output.position = patch[pointId].position;

	// Pass the color
	output.texCoord = patch[pointId].texCoord;
	output.normal = patch[pointId].normal;
	output.tangent = patch[pointId].tangent;

	return patch[pointId];
}