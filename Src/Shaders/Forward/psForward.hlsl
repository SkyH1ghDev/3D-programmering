Texture2D testTexture: register(t0);
SamplerState testSampler;

cbuffer ConstBuffer: register(b0)
{
	float4 lightColour;
	float4 lightPosition;
	float4 eyePosition;
	float ambientLightIntensity;
	float generalLightIntensity;
	float shininess;
}

struct PixelShaderInput
{
	float4 worldPosition : POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

float4 main(PixelShaderInput input): SV_TARGET
{
	float4 ambientComponent = lightColour * ambientLightIntensity;

	float4 lightDirection = normalize(lightPosition - input.worldPosition);
	float diffuseIntensity = max(dot(input.normal, lightDirection), 0.0f);
	float4 diffuseComponent = lightColour * diffuseIntensity;

	// BLINN-PHONG
	
	float4 viewDirection = normalize(eyePosition - input.worldPosition);
	float4 halfway = normalize(viewDirection + lightDirection);
	float specularIntensity = pow(max(dot(input.normal, halfway), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;

	// PHONG

	/*
	float4 reflection = reflect(-lightDirection, input.normal);
	float4 vectorToEye = normalize(eyePosition - input.worldPosition);
	float specularIntensity = pow(max(dot(vectorToEye, reflection), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;
	*/
	
	float4 result = (ambientComponent + diffuseComponent + specularComponent) * testTexture.Sample(testSampler, input.uv);
	return result; 
}