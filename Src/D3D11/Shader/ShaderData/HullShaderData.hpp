#pragma once
#include <DirectXMath.h>

namespace DX = DirectX;

_declspec(align(16))
class HullShaderData
{
public:
    DX::XMFLOAT4 MeshPosition;
    DX::XMFLOAT4 CameraPosition;
};
