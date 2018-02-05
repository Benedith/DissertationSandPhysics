cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
}



struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEX;
	float3 normal : NORMAL;
}



struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEX;
	float3 normal : NORMAL
}



PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);

	output.normal = normalize(output.normal);

	return output;
}