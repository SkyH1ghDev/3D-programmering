#pragma once

#include <DirectXMath.h>

#include "IConfigBase.hpp"

namespace DX = DirectX;

class LightConfig : IConfigBase
{
public:
    DX::XMFLOAT4 LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
    DX::XMFLOAT4 LightPosition = {0.0f, -7.0f, -10.0f, 1.0f};
    float AmbientLightIntensity = 0.2f;
    float GeneralLightIntensity = 1.0f;
};
