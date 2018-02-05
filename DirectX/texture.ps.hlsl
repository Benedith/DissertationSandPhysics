Texture2D shader_texture;
SamplerState sample_type;



struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEX;
};



float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	return shader_texture.Sample(sample_type, input.tex);
}