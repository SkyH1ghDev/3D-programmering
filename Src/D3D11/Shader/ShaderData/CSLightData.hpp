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
