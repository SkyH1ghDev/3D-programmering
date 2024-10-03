#pragma once

#include <array>
#include <DirectXMath.h>

#include "ProjectionMatrixConfig.hpp"

namespace DX = DirectX;


class MatrixCreator
{
public:
    MatrixCreator() = default;

    DX::XMMATRIX CreateInitialWorldMatrixTransposed();
    DX::XMMATRIX CreateInitialViewMatrixTransposed();
    DX::XMMATRIX CreateInitialProjectionMatrix();
    DX::XMMATRIX CreateInitialViewProjectionMatrixTransposed();
    DX::XMMATRIX CreateTranslationMatrix(const float& x, const float& y, const float& z);
};
