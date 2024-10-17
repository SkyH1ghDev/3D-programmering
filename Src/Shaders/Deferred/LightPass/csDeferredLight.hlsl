RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> colourGBuffer : register(t2);
Texture2D<float4> ambientGBuffer : register(t3);
Texture2D<float4> diffuseGBuffer : register(t4);
Texture2D<float4> specularGBuffer : register(t5);

struct SpotLight
{
	float4x4 viewProjectionMatrix;
	float4 colour;
	float4 direction;
	float4 position;
	float angle;
};

StructuredBuffer<SpotLight> SpotLights : register(t6);
Texture2DArray<float> shadowMaps : register(t7);
sampler shadowMapSampler : register(s0);

cbuffer CSBuffer : register(b0)
{
	float4 pointLightColour;
	float4 pointLightPosition;
	float4 camPosition;
	float ambientLightIntensity;
	float generalLightIntensity;
	int resultMode;
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
	float4 halfway = normalize(lightDirection + camDirection);
	float specularIntensity = pow(max(dot(normalize(fragmentNormal), halfway), 0.0f), shininess) * generalLightIntensity;
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

	float4 totalAmbientComponent = float4(0.1f, 0.1f, 0.1f, 0.1f);
	float4 totalDiffuseComponent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 totalSpecularComponent = float4(0.0f, 0.0f, 0.0f, 0.0f);

	const int numLights = 4;
	
	for (int lightIndex = 0; lightIndex < numLights; ++lightIndex)
	{
		SpotLight spotLight = SpotLights[lightIndex];
        
		float4 clipSpacePosition = mul(fragmentPosition, spotLight.viewProjectionMatrix);
		float4 ndcSpacePosition = clipSpacePosition / clipSpacePosition.w;
		float depthValueBetweenLightAndFragment = ndcSpacePosition.z - 0.00001f;
        
		float3 shadowMapUV = float3(ndcSpacePosition.x * 0.5f + 0.5f, ndcSpacePosition.y * -0.5f + 0.5f, lightIndex);

		// Ambient
		float4 spotLightAmbient = GetAmbientComponent(spotLight.colour, ambientLightIntensity);
        
		float4 lightVector = spotLight.position - fragmentPosition;
		float4 lightDirection = normalize(lightVector);
		
		// Diffuse
		float4 spotLightDiffuse = GetDiffuseComponent(spotLight.colour, lightDirection, generalLightIntensity, fragmentNormal);
	
		// Specular
		float4 spotLightSpecular = GetSpecularComponent(spotLight.colour, lightDirection, generalLightIntensity, specularExponent, camPosition, fragmentPosition, fragmentNormal);
        	
		float actualDepthValue = shadowMaps.SampleLevel(shadowMapSampler, shadowMapUV, 0);

		float4 lightDistance = spotLight.position - fragmentPosition;

		float distanceScalingFactor = 1 / sqrt(dot(lightDistance, lightDistance));
		
		if (depthValueBetweenLightAndFragment < actualDepthValue && abs(dot(normalize(lightDirection), normalize(spotLight.direction))) > cos(radians(spotLight.angle)))
		{
			totalAmbientComponent += spotLightAmbient;
			totalDiffuseComponent += spotLightDiffuse * distanceScalingFactor;
			totalSpecularComponent += spotLightSpecular * distanceScalingFactor;
		}
	}
	
	// Ambient
	//totalAmbientComponent += GetAmbientComponent(pointLightColour, ambientLightIntensity);

	//float4 lightDistance = spotlight - fragmentPosition;
	//float4 lightDirection = normalize(lightDistance);
	
	// Diffuse
	//totalDiffuseComponent += GetDiffuseComponent(pointLightColour, lightDirection, generalLightIntensity, fragmentNormal);

	// Specular
	//totalSpecularComponent += GetSpecularComponent(pointLightColour, lightDirection, generalLightIntensity, specularExponent, camPosition, fragmentPosition, fragmentNormal);

	//float distanceScalingFactor = 1 / sqrt(dot(lightDistance, lightDistance));

	float4 result;
	[call] switch(resultMode)
	{
		case 0:
			result = (totalAmbientComponent * ambientFactor + totalDiffuseComponent * diffuseFactor + totalSpecularComponent * specularFactor) * colour;
			break;
		
		case 1:
			result = fragmentPosition;
			break;

		case 2:
			result = fragmentNormal;
			break;

		case 3:
			result = colour;
			break;

		case 4:
			result = ambientFactor;
			break;

		case 5:
			result = diffuseFactor;
			break;

		case 6:
			result = specularFactor;
			break;
		
		default:
			result = (totalAmbientComponent * ambientFactor + totalDiffuseComponent * diffuseFactor + totalSpecularComponent * specularFactor) * colour;
			break;
	}
	result.rgb = saturate(result.rgb);

    backBufferUAV[DTid.xy] = result;
}


