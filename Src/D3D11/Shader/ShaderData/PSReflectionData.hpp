#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

__declspec(align(16))
class PSReflectionData
{
public:
    float SpecularExponent;
    DX::XMFLOAT4 CameraPosition;
};
