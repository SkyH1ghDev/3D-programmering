cbuffer ConstBuffer : register(b0)
{
	matrix worldMatrix;
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
	float4 position : SV_POSITION;
	float4 worldPosition : WORLD_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
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