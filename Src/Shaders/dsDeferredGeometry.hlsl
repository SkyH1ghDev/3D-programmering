cbuffer cameraBuffer : register(b0)
{
    float4x4 viewProjectionMatrix;
};

struct DomainShaderOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct HullShaderOutput
{
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]

DomainShaderOutput main(HS_CONSTANT_DATA_OUTPUT input, float3 barycentric : SV_DomainLocation, const OutputPatch<HullShaderOutput, NUM_CONTROL_POINTS> patch)
{
    DomainShaderOutput output;

    output.worldPosition = float4(patch[0].worldPosition.xyz * barycentric.x + patch[1].worldPosition.xyz * barycentric.y + patch[2].worldPosition.xyz * barycentric.z, 1.0f);
    output.normal = float4(normalize(patch[0].normal.xyz * barycentric.x + patch[1].normal.xyz * barycentric.y + patch[2].normal.xyz * barycentric.z), 0.0f);
    output.uv = patch[0].uv * barycentric.x + patch[1].uv * barycentric.y + patch[2].uv * barycentric.z;

    output.position = mul(float4(output.worldPosition.xyz, 1), viewProjectionMatrix);

    return output;
}