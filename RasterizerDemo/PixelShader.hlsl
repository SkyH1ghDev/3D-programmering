Texture2D testTexture: register(t0);
SamplerState testSampler;

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float4 worldPosition : WORLD_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float ambientLightIntensity = 0.1f;
	float4 lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	
	float4 ambientComponent = lightColour * ambientLightIntensity;

	float4 lightPosition = {0.0f, 0.0f, -5.0f, 1.0f};
	float4 eyePosition = {0.0f, 0.0f, -5.0f, 1.0f};
	float4 lightDirection = normalize(lightPosition - input.worldPosition);
	float4 vectorToEye = normalize(eyePosition - input.worldPosition);
	float diffuseIntensity = max(dot(input.normal, lightDirection), 0.0f);
	float4 diffuseComponent = lightColour * diffuseIntensity;

	// BLINN-PHONG

	/*
	float4 viewDirection = normalize(eyePosition - input.worldPosition);
	float4 halfway = normalize(viewDirection + lightDirection);
	float shininess = 10000.0f;
	float specularIntensity = pow(max(dot(input.normal, halfway), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;
	*/
	
	float4 reflection = reflect(-lightDirection, input.normal);
	float shininess = 10000.0f;
	float specularIntensity = pow(max(dot(vectorToEye, reflection), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;
	
	float4 result = (ambientComponent + diffuseComponent + specularComponent) * testTexture.Sample(testSampler, input.uv);
	return result; 
    //return float4(result);
}