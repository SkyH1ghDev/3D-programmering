#include "Math.hpp"
#include <cmath>

// PI
#define M_PI 3.14159265358979323846

std::array<float, 4> Math::OscillateBetweenPoints(float time, std::array<float, 4> pos1, std::array<float, 4> pos2)
{
	std::array<float, 4> difference = {pos1[0] - pos2[0], pos1[1] - pos2[1], pos1[2] - pos2[2], 1.0f};
	float length = std::sqrt(difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2]);

	double angle = std::fmod(time * 2 * M_PI, 2 * M_PI);

	float tCos = std::cos(static_cast<float>(angle));

	return{pos1[0] + difference[0] * (tCos + 1) / 2,
		      pos1[1] + difference[1] * (tCos + 1) / 2,
		      pos1[2] + difference[2] * (tCos + 1) / 2,
			  1.0f};
}
