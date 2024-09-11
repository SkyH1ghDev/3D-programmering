#pragma once

#include <array>
#include <DirectXMath.h>

#include "ProjectionMatrixConfig.hpp"

namespace DX = DirectX;


class MatrixCreator
{
public:
    MatrixCreator() = default;

    DX::XMMATRIX CreateWorldXMMATRIX(const float& angle);
    DX::XMMATRIX CreateWorldXMMATRIX();

    DX::XMFLOAT4X4 CreateWorldXMFLOAT4X4(const float& angle);
    DX::XMFLOAT4X4 CreateWorldXMFLOAT4X4();
    
    DX::XMMATRIX CreateViewXMMATRIX(const DX::XMFLOAT4 &camPos, const DX::XMFLOAT4 &directionVector);
    DX::XMMATRIX CreateViewXMMATRIX();
    
    DX::XMMATRIX CreateProjectionXMMATRIX(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ);
    DX::XMMATRIX CreateProjectionXMMATRIX(const ProjectionMatrixConfig& projectionMatrixConfig);

    DX::XMFLOAT4X4 CreateTranslationMatrixXMFLOAT4X4(const float& x, const float& y, const float& z);
};
