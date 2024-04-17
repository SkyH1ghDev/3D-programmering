#pragma once

#include <array>
#include "IConfigBase.hpp"

class ViewMatrixConfig : public IConfigBase
{
public:
    std::array<float, 4> GetCamPosition() const { return this->_camPosition; }
    std::array<float, 4> GetFocusPosition() const { return this->_focusPosition; }
    
private:
    std::array<float, 4> _camPosition = {0.0f, 10.0f, -40.0f, 1.0f};
    std::array<float, 4> _focusPosition = {1.0f, 0.0f, 0.0f, 1.0f};
};
