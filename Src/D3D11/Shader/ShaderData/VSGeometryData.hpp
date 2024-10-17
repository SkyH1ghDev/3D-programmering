#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

_declspec(align(16))
class VSGeometryData
{
public:
    DX::XMFLOAT4X4 WorldMatrix;
    DX::XMFLOAT4X4 ViewProjectionMatrix;
};
