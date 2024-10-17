#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

__declspec(align(16))
class GSParticleData
{
public:
    DX::XMFLOAT4X4 ViewProjectionMatrix;
    DX::XMFLOAT4 CameraPosition;
};
