﻿#pragma once

#include <array>

class Math
{
public:
	static std::array<float, 4> OscillateBetweenPoints(float time, std::array<float, 4> pos1, std::array<float, 4> pos2);
};
