#pragma once

#include <DirectXMath.h>

#include "IConfigBase.hpp"

namespace DX = DirectX;

class LightConfig : IConfigBase
{
public:
    const DX::XMFLOAT4& GetLightColour() const { return this->_lightColour; }
    const DX::XMFLOAT4& GetLightPosition() const { return this->_lightPosition; }
    const float& GetAmbientLightIntensity() const { return this->_ambientLightIntensity; }
    const float& GetGeneralLightIntensity() const { return this->_generalLightIntensity; }

    
private:
    
    DX::XMFLOAT4 _lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
    DX::XMFLOAT4 _lightPosition = {0.0f, -7.0f, -10.0f, 1.0f};
    float _ambientLightIntensity = 0.2f;
    float _generalLightIntensity = 2.0f;
};
