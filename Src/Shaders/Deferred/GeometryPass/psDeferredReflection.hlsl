Texture2D colourTexture : register(t0);
Texture2D ambientTexture : register(t1);
Texture2D diffuseTexture : register(t2);
Texture2D specularTexture : register(t3);
TextureCube reflectionTexture : register(t4);
SamplerState sState;

cbuffer PSBuffer : register(b0)
{
    float specularExponent;
    float4 cameraPosition;
}

struct PSInput
{
    float4 position : SV_POSITION;
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
    output.ambient = float4(1, 1, 1, 1);
    output.diffuse = diffuseTexture.Sample(sState, input.uv);
    output.specular = specularTexture.Sample(sState, input.uv);
    output.specular.w = specularExponent;

    float3 normal = normalize(input.normal.xyz);
    float3 incomingView = normalize(input.worldPosition - cameraPosition);
    float3 reflectedView = reflect(incomingView, normal);
    float4 sampledValue = reflectionTexture.Sample(sState, reflectedView);

    output.colour = sampledValue;
    
    return output;
}
