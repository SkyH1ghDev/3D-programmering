#pragma once

#include <array>
#include "ConfigBase.hpp"

class ViewMatrixConfig : public ConfigBase
{
public:
    const std::array<float, 4> EyePos = {0.0f, 10.0f, -15.0f, 1.0f};
};
