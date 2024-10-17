cbuffer GSBuffer : register(b0)
{
    float4x4 ViewProjectionMatrix;
    float4 CameraPosition;
};

struct GeometryShaderOutput
{
    float4 Position : SV_POSITION;
    float4 Colour : COLOUR;
};

[maxvertexcount(6)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<GeometryShaderOutput> output)
{
    float3 frontVector = normalize(CameraPosition.xyz - input[0].xyz);
    float3 rightVector = normalize(cross(frontVector, float3(0.0f, 1.0f, 0.0f)));
    float3 upVector = normalize(cross(frontVector, rightVector)) * 0.5f;
    rightVector = normalize(cross(frontVector, upVector)) * 0.5f;

    GeometryShaderOutput TopLeft, TopRight, BottomLeft, BottomRight;

    float4 colour = float4(0.0f, 1.0f, 0.0f, 0.0f);
    
    //Top-Left
    TopLeft.Position = mul(float4(float3(input[0].xyz) - rightVector + upVector, 1.0f), ViewProjectionMatrix);

    //Bottom-Right
    BottomRight.Position = mul(float4(float3(input[0].xyz) + rightVector - upVector, 1.0f), ViewProjectionMatrix);

    //Bottom-Left
    BottomLeft.Position = mul(float4(float3(input[0].xyz) - rightVector - upVector, 1.0f), ViewProjectionMatrix);

    //Top-Right
    TopRight.Position = mul(float4(float3(input[0].xyz) + rightVector + upVector, 1.0f), ViewProjectionMatrix);

    TopLeft.Colour = BottomRight.Colour = BottomLeft.Colour = TopRight.Colour = colour;

    output.Append(TopLeft);
    output.Append(BottomRight);
    output.Append(BottomLeft);

    output.RestartStrip();

    output.Append(TopLeft);
    output.Append(TopRight);
    output.Append(BottomRight);
}