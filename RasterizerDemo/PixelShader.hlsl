Texture2D testTexture: register(t0);
SamplerState testSampler;
float4 eyePosition = {0.0f, 0.0f, -5.0f, 1.0f};

struct Material
{
	float4 normal : MATERIAL_NORMAL;
	float4 diffuseColour : DIFFUSE_COLOUR;
	float specularIntensity : SPECULAR_INSTENSITY;
	float specularExponent : SPECULAR_EXPONENT;
};

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

Material PrepareMaterial(float4 normal, float2 UV)
{
	Material material;
	material.normal = normalize(normal);
	material.diffuseColour = testTexture.Sample(testSampler, UV);
	material.specularExponent = 1.0f;
	material.specularIntensity = 1.0f;
	return material;
}

float4 main(PixelShaderInput input, Material material) : SV_TARGET
{
	float ambientLightIntensity = 0.1f;
	float4 lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	
	float4 ambientComponent = lightColour * ambientLightIntensity;

	float4 lightPosition = {0.0f, 0.0f, -20.0f, 1.0f};
	float4 lightDirection = normalize(lightPosition - input.position);
	float diffuseIntensity = min(max(dot(input.normal, lightDirection), 0.0f), 1.0f);
	float4 diffuseComponent = lightColour * diffuseIntensity;

	float4 viewDirection = normalize(eyePosition - input.position);
	float4 halfway = normalize(viewDirection + lightDirection);
	float shininess = 100.0f;
	float specularIntensity = pow(max(dot(input.normal, halfway), 0.0f), shininess);
	float4 specularComponent = lightColour * specularIntensity;
	float4 result = (ambientComponent + diffuseComponent) * testTexture.Sample(testSampler, input.uv);
	return input.position;
	//return float4(testTexture.Sample(testSampler, input.uv).xyz, 1.0f);
}