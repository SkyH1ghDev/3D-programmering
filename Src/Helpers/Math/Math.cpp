#include "Math.hpp"
#include <cmath>

std::array<float, 4> Math::OscillateBetweenPoints(float time, std::array<float, 4> pos1, std::array<float, 4> pos2)
{
	std::array<float, 4> difference = {pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2], 1.0f};

	double angle = std::fmod(time * 2 * DX::XM_PI, 2 * DX::XM_PI);

	float tSin = std::sin(static_cast<float>(angle));

	return{pos1[0] + difference[0] * (tSin + 1) / 2,
		      pos1[1] + difference[1] * (tSin + 1) / 2,
		      pos1[2] + difference[2] * (tSin + 1) / 2,
			  1.0f};
}
