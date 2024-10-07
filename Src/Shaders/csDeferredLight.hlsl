RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> colourGBuffer : register(t2);
Texture2D<float4> ambientGBuffer : register(t3);
Texture2D<float4> diffuseGBuffer : register(t4);
Texture2D<float4> specularGBuffer : register(t5);

cbuffer ConstBuffer : register(b0)
{
	float4 lightColour;
	float4 lightPosition;
	float4 camPosition;
	float ambientLightIntensity;
	float generalLightIntensity;
}

float4 GetAmbientComponent(float4 lightColour, float ambientLightIntensity)
{
	return lightColour * ambientLightIntensity;
}

float4 GetDiffuseComponent(float4 lightColour, float4 lightDirection, float generalLightIntensity, float4 fragmentNormal)
{
	float diffuseIntensity = max(dot(fragmentNormal, lightDirection), 0.0f) * generalLightIntensity;
	return lightColour * diffuseIntensity;
}

float4 GetSpecularComponent(float4 lightColour, float4 lightDirection, float generalLightIntensity, float shininess, float4 camPosition, float4 fragmentPosition, float4 fragmentNormal)
{
	float4 camDirection = normalize(fragmentPosition + camPosition);
	float4 halfway = lightDirection + camDirection / sqrt(dot(lightDirection + camDirection, lightDirection + camDirection));
	float specularIntensity = pow(max(dot(fragmentNormal, halfway), 0.0f), shininess) * generalLightIntensity;
	return lightColour * specularIntensity;
}

[numthreads(8,8,1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	float4 fragmentPosition = float4(positionGBuffer[DTid.xy].xyz, 1.0f);
	float4 fragmentNormal = float4(normalGBuffer[DTid.xy].xyz, 0.0f);
	float4 colour = float4(colourGBuffer[DTid.xy].xyz, 1.0f);
	float4 ambientFactor = float4(ambientGBuffer[DTid.xy].xyz, 1.0f);
	float4 diffuseFactor = float4(diffuseGBuffer[DTid.xy].xyz, 1.0f);
	float4 specularFactor = specularGBuffer[DTid.xy].xyzw;
	float specularExponent = specularFactor.w;

	// Ambient
	float4 ambientComponent = GetAmbientComponent(lightColour, ambientLightIntensity);

	float4 lightDistance = lightPosition - fragmentPosition;
	float4 lightDirection = normalize(lightDistance);
	
	// Diffuse
	float4 diffuseComponent = GetDiffuseComponent(lightColour, lightDirection, generalLightIntensity, fragmentNormal);

	// Specular
	float4 specularComponent = GetSpecularComponent(lightColour, lightDirection, generalLightIntensity, specularExponent, camPosition, fragmentPosition, fragmentNormal);

	float distanceScalingFactor = 1 / sqrt(dot(lightDistance, lightDistance));
	float4 result = (ambientComponent * ambientFactor + distanceScalingFactor * (diffuseComponent * diffuseFactor + specularComponent * specularFactor)) * colour;

    backBufferUAV[DTid.xy] = result;
}


