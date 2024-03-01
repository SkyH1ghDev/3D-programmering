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
	output.normal = normalize(float4(mul(input.normal, worldMatrix).xyz, 0.0f));
	output.uv = input.uv;
	return output;
}