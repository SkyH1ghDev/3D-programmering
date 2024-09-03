cbuffer WorldMatrixBuffer : register(b0)
{
	matrix worldMatrix;
};

cbuffer ViewProjectionMatrixBuffer : register(b1)
{
	matrix viewProjectionMatrix;
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
	output.worldPosition = mul(input.position, worldMatrix);
	output.position = mul(output.worldPosition, viewProjectionMatrix);
	output.normal = normalize(mul(input.normal, worldMatrix));
	output.uv = input.uv;
	return output;
}