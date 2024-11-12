#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

__declspec(align(16))
class CSParticleData
{
public:
    DX::XMFLOAT4 randomizedVector;
};
