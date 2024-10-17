#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

__declspec(align(16))
class CSLightData
{
public:
	DX::XMFLOAT4 LightColour;
	DX::XMFLOAT4 LightPosition; 
	DX::XMFLOAT4 CamPosition;
	float AmbientLightIntensity;
	float GeneralLightIntensity;
	int outputMode;
	
};

class CSLightData2
{
public:
	DX::XMFLOAT4X4 vpMatrix;
	DX::XMFLOAT4 colour;
	DX::XMFLOAT4 direction;
	DX::XMFLOAT4 position;
	float angle = 0.0f;
	
};
