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
	matrix instance_world_matrix : INSTANCE;
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

	matrix instanceWorldMatrix = input.instance_world_matrix;

    output.position = mul(input.position, instanceWorldMatrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);
    
    output.color = input.color;
    
    return output;
}