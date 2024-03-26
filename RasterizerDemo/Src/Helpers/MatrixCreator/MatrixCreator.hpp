#pragma once

#include <DirectXMath.h>
#include <array>

#include "ProjectionMatrixConfig.hpp"
#include "ViewMatrixConfig.hpp"
#include "WorldMatrixConfig.hpp"

class MatrixCreator
{
public:
    MatrixCreator() = default;

    DirectX::XMMATRIX CreateWorldMatrix(const float& angle);
    DirectX::XMMATRIX CreateWorldMatrix(const WorldMatrixConfig& worldMatrixConfig);
    
    DirectX::XMMATRIX CreateViewMatrix(const std::array<float, 4>& eyePos);
    DirectX::XMMATRIX CreateViewMatrix(const ViewMatrixConfig& viewMatrixConfig);
    
    DirectX::XMMATRIX CreateProjectionMatrix(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ);
    DirectX::XMMATRIX CreateProjectionMatrix(const ProjectionMatrixConfig& projectionMatrixConfig);
};
