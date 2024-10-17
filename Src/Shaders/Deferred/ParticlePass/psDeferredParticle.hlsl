struct PixelShaderInput
{
	float4 Position : SV_POSITION;
	float4 Colour : COLOUR;
};

float4 main(PixelShaderInput input): SV_TARGET
{
	return input.Colour; 
}
