#pragma once

#include <DirectXMath.h>
#include "Camera.hpp"

namespace DX = DirectX;


class MatrixCreator
{
public:
    MatrixCreator() = default;

    // Create Initial Matrices
    DX::XMMATRIX CreateWorldMatrix(const DX::XMFLOAT4& position);
    DX::XMMATRIX CreateViewMatrix(const DX::XMFLOAT4& cameraPosition, const DX::XMFLOAT4& viewDirection, const DX::XMFLOAT4& upDirection);
    DX::XMMATRIX CreateProjectionMatrix(const ProjectionInfo& projInfo);
    DX::XMMATRIX CreateViewProjectionMatrix(const Camera& camera);

    // Create Matrices
    DX::XMMATRIX CreateTranslationMatrix(const float& x, const float& y, const float& z);
    DX::XMMATRIX CreateRotationMatrixX(const float& rotationX);
    DX::XMMATRIX CreateRotationMatrixY(const float& rotationY);
    DX::XMMATRIX CreateRotationMatrixZ(const float& rotationZ);
};
