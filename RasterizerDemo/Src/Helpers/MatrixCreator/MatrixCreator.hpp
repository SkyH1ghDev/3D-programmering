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

    DX::XMMATRIX CreateWorldXMMATRIX(const float& angle);
    DX::XMMATRIX CreateWorldXMMATRIX();

    DX::XMFLOAT4X4 CreateWorldXMFLOAT4X4(const float& angle);
    DX::XMFLOAT4X4 CreateWorldXMFLOAT4X4();
    
    DX::XMMATRIX CreateViewXMMATRIX(const std::array<float, 4>& camPos, const std::array<float, 4>& focalPoint);
    DX::XMMATRIX CreateViewXMMATRIX();
    
    DX::XMMATRIX CreateProjectionXMMATRIX(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ);
    DX::XMMATRIX CreateProjectionXMMATRIX(const ProjectionMatrixConfig& projectionMatrixConfig);
};
