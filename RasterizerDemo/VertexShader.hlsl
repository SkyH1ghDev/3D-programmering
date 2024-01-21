cbuffer ConstBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewProjectionMatrix;
};



struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = mul(mul(float4(/*input.position*/0.0f, 0.0f, 3.0f, 1.0f), worldMatrix), viewProjectionMatrix);
	output.normal = normalize(mul(mul(float4(input.normal, 0.0f), worldMatrix), viewProjectionMatrix));
	output.uv = input.uv;
	return output;
}