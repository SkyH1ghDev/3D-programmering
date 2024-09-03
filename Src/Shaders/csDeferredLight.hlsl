RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> colourGBuffer : register(t2);

cbuffer ConstBuffer : register(b0)
{
	float4 lightColour;
	float4 lightPosition;
	float4 eyePosition;
	float ambientLightIntensity;
	float shininess;
}

[numthreads(8,8,1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	// Works in renderdoc but not here
    float4 position = float4(positionGBuffer[DTid.xy].xyz, 1.0f);
    float4 normal = float4(normalGBuffer[DTid.xy].xyz, 0.0f);
    float4 colour = float4(colourGBuffer[DTid.xy].xyz, 1.0f);

	// Ambient
	float4 ambientComponent = lightColour * ambientLightIntensity;

	// Diffuse
	float4 lightDirection = normalize(lightPosition - position);
	float diffuseIntensity = max(dot(normal, lightDirection), 0.0f);
	float4 diffuseComponent = lightColour * diffuseIntensity;

	// Specular
	float4 viewDirection = normalize(eyePosition - position);
	float4 halfway = normalize(viewDirection + lightDirection);
	float specularIntensity = pow(max(dot(normal, halfway), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;

	float4 result = (ambientComponent + diffuseComponent + specularComponent) * colour;

    backBufferUAV[DTid.xy] = result;
}