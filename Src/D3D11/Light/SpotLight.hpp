#pragma once

#include <DirectXMath.h>

#include "Camera.hpp"
#include "CSLightData.hpp"

namespace DX = DirectX;

class SpotLight
{
public:
    SpotLight(HRESULT& hr, ID3D11Device* device, const ProjectionInfo& projectionInfo, const DX::XMFLOAT4& position, const DX::XMFLOAT4& lightColour, const float& angle);

public:
    Camera& GetCamera();
    DX::XMFLOAT4 GetLightColour() const;
    float GetAngle() const;

    CSLightData2 GetSpotlightData();
    
private:
    Camera _camera;
    DX::XMFLOAT4 _lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
    float _angle = 10.0f;
};
