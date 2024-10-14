#pragma once
#include <DirectXMath.h>

namespace DX = DirectX;

_declspec(align(16))
class DomainShaderData
{
public:
    DX::XMFLOAT4X4 ViewProjectionMatrix;
};
