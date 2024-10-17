cbuffer VSBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewProjectionMatrix;
};


struct VertexShaderInput
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

struct VertexShaderOutput
{
	// Passed to Pixel Shader
	float4 worldPosition : WORLD_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;

	// Set system values
	float4 position : SV_POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.worldPosition = mul(input.position, WorldMatrix);
	output.position = mul(output.worldPosition, ViewProjectionMatrix);
	output.normal = normalize(mul(input.normal, WorldMatrix));
	output.uv = input.uv;
	return output;
}