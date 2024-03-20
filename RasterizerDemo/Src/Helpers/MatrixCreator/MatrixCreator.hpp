#pragma once

#include <DirectXMath.h>
#include <array>

class MatrixCreator
{
public:
    MatrixCreator() = default;

    DirectX::XMMATRIX CreateWorldMatrix(const float& angle);
    DirectX::XMMATRIX CreateViewMatrix(const std::array<float, 4>& eyePos);
    DirectX::XMMATRIX CreateProjectionMatrix(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ);
};
