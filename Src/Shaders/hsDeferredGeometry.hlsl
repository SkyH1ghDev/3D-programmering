cbuffer PositionBuffer : register(b0)
{
    float4 meshPosition;
    float4 cameraPosition;
}

struct VertexShaderOutput
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

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(InputPatch<VertexShaderOutput, NUM_CONTROL_POINTS> ip)
{
    HS_CONSTANT_DATA_OUTPUT output;

    float minDistance = 5.0f;
    float maxDistance = 25.0f;

    float distanceFromCamera = length(cameraPosition - meshPosition);

    float tessFactor = clamp(32 * (maxDistance - distanceFromCamera) / (maxDistance - minDistance), 1, 32);
    
    output.EdgeTessFactor[0] = output.EdgeTessFactor[1] = output.EdgeTessFactor[2] = output.InsideTessFactor = tessFactor;

    return output;
}

struct HullShaderOutput
{
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]

HullShaderOutput main(InputPatch<VertexShaderOutput, NUM_CONTROL_POINTS> ip, uint i : SV_OutputControlPointID)
{
    HullShaderOutput output;

    output.worldPosition = ip[i].worldPosition;
    output.normal = ip[i].normal;
    output.uv = ip[i].uv;

    return output;
}