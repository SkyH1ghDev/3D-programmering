struct PSInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct PSOutput
{
    float4 worldPosition : SV_Target0;
    float4 normal : SV_Target1;
    float4 uv : SV_Target2;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    output.worldPosition = input.worldPosition;
    output.normal = input.normal;
    output.uv = float4(input.uv, 255, 255);

    return output;
}