#pragma once

#include <DirectXMath.h>
#include <array>

namespace DX = DirectX;

class Math
{
public:
	static DX::XMFLOAT4 OscillateBetweenPoints(float time, DX::XMFLOAT4 pos1, DX::XMFLOAT4 pos2);
};
