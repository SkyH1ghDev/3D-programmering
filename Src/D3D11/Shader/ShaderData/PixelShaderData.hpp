#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

class PixelShaderData
{
public:
    
	DX::XMFLOAT4 LightColour;
	DX::XMFLOAT4 LightPosition; 
	DX::XMFLOAT4 EyePosition;
	float AmbientLightIntensity; 
	float Shininess;

private:
	char _padding[8] = {};
};
