Texture2D colourTexture : register(t0);
Texture2D ambientTexture : register(t1);
Texture2D diffuseTexture : register(t2);
Texture2D specularTexture : register(t3);
SamplerState sState;

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
    float4 ambient : SV_Target3;
    float4 diffuse : SV_Target4;
    float4 specular : SV_Target5;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    output.worldPosition = input.worldPosition;
    output.normal = input.normal;
    output.colour = colourTexture.Sample(sState, input.uv);
    output.ambient = ambientTexture.Sample(sState, input.uv);
    output.diffuse = diffuseTexture.Sample(sState, input.uv);
    output.specular = specularTexture.Sample(sState, input.uv);

    return output;
}