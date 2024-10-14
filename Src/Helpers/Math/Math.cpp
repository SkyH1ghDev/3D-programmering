#include "Math.hpp"
#include <cmath>

DX::XMFLOAT4 Math::OscillateBetweenPoints(float time, DX::XMFLOAT4 pos1, DX::XMFLOAT4 pos2)
{
	DX::XMFLOAT4 difference = {pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z, 1.0f};

	double angle = std::fmod(time * 2 * DX::XM_PI, 2 * DX::XM_PI);

	float tSin = std::sin(static_cast<float>(angle));

	return{pos1.x + difference.x * (tSin + 1) / 2,
		      pos1.y + difference.y * (tSin + 1) / 2,
		      pos1.z + difference.z * (tSin + 1) / 2,
			  1.0f};
}
