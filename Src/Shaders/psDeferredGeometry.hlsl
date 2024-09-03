Texture2D testTexture : register(t0);
SamplerState sampler1;

struct PSInput
{
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct PSOutput
{
    float4 worldPosition : SV_Target0;
    float4 normal : SV_Target1;
    float4 colour : SV_Target2;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    output.worldPosition = input.worldPosition;
    output.normal = input.normal;
    output.colour = testTexture.Sample(sampler1, input.uv);

    return output;
}