cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};



cbuffer InstanceBuffer
{
	matrix instance_world_matrix;
};




struct VertexInputType
{
	float4 position : POSITION;
	float4 color : TEXCOORD0;
	float3 instance_position : TEXCOORD1;
	float3 instance_rotation : TEXCOORD2;
};



struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};



PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	matrix translation =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		input.instance_position.x, input.instance_position.y, input.instance_position.z, 1.0f
	}

	matrix rotationAroundY =
	{
		cos(input.instance_rotation.y, 0.0f, -sin(input.instance_rotation.y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(input.instance_rotation.y), 0.0f, cos(input.instance_rotation.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	}

	matrix <float, 4, 4> composition;

	composition = mul(rotationAroundY, translation);


	output.position = mul(input.position, composition);
	output.position = mul(output.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);

	output.color = input.color;

	return output;
}