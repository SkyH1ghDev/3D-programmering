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
    DX::XMMATRIX CreateOrthographicProjectionMatrix();
    DX::XMMATRIX CreateViewProjectionMatrix(const Camera& camera);
    DX::XMMATRIX CreateOrthographicViewProjectionMatrix(const Camera& camera);

    // Create Matrices
    DX::XMMATRIX CreateTranslationMatrix(const float& x, const float& y, const float& z);
    DX::XMMATRIX CreateRotationMatrixX(const float& yaw);
    DX::XMMATRIX CreateRotationMatrixY(const float& pitch);
    DX::XMMATRIX CreateRotationMatrixZ(const float& roll);
    DX::XMMATRIX CreateRotationMatrixAxis(const float& angle, const DX::XMVECTOR& axis);
};
