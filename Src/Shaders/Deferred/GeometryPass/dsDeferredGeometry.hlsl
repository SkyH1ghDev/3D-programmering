cbuffer DSBuffer : register(b0)
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

struct DomainShaderInput
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

float3 BarycentricInterpolatePosition(OutputPatch<DomainShaderInput, NUM_CONTROL_POINTS> patch, float3 u, float3 v, float3 w)
{
    return u * patch[0].worldPosition.xyz + v * patch[1].worldPosition.xyz + w * patch[2].worldPosition.xyz;
}

float3 BarycentricInterpolateNormal(OutputPatch<DomainShaderInput, NUM_CONTROL_POINTS> patch, float3 u, float3 v, float3 w)
{
    return u * patch[0].normal.xyz + v * patch[1].normal.xyz + w * patch[2].normal.xyz;
}

float3 OrthogonalProjection(float3 q, float3 p, float3 n)
{
    return q - dot((q - p), n) * n;
}

[domain("tri")]

DomainShaderOutput main(HS_CONSTANT_DATA_OUTPUT input, float3 barycentric : SV_DomainLocation, const OutputPatch<DomainShaderInput, NUM_CONTROL_POINTS> patch)
{
    DomainShaderOutput output;

    float u = barycentric.x;
    float v = barycentric.y;
    float w = barycentric.z;

    float3 puv = BarycentricInterpolatePosition(patch, u, v, w);

    float shapeFactor = 0.75f;

    float3 orthoProjected = float3
    (
        u * OrthogonalProjection(puv, patch[0].worldPosition.xyz, patch[0].normal.xyz) +
        v * OrthogonalProjection(puv, patch[1].worldPosition.xyz, patch[1].normal.xyz) +
        w * OrthogonalProjection(puv, patch[2].worldPosition.xyz, patch[2].normal.xyz)
    );

    output.worldPosition = float4((1 - shapeFactor) * puv + shapeFactor * orthoProjected, 1.0f);
    output.position = mul(float4(output.worldPosition.xyz, 1.0f), viewProjectionMatrix);
    output.normal = float4(BarycentricInterpolateNormal(patch, u, v, w), 1.0f);
    output.uv = patch[0].uv * barycentric.x + patch[1].uv * barycentric.y + patch[2].uv * barycentric.z;
    
    return output;
}