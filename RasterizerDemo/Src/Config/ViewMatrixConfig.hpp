#pragma once

#include <array>
#include "IConfigBase.hpp"

class ViewMatrixConfig : public IConfigBase
{
public:
    std::array<float, 4> GetEyePosition() const { return this->_eyePos; }
    
private:
    std::array<float, 4> _eyePos = {0.0f, 10.0f, -40.0f, 1.0f};
};
