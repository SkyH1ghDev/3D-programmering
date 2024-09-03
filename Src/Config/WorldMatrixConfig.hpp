#pragma once

#include "IConfigBase.hpp"

class WorldMatrixConfig : public IConfigBase
{
public:
    float GetInitialAngle() const { return this->_initalAngle; }
    
private:
    float _initalAngle = 0.0f;
};
