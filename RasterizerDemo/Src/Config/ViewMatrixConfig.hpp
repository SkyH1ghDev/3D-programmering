#pragma once

#include <array>
#include <DirectXMath.h>

#include "IConfigBase.hpp"
namespace DX = DirectX;

class ViewMatrixConfig : public IConfigBase
{
public:
    DX::XMFLOAT4 GetCamPosition() const { return this->_camPosition; }
    DX::XMFLOAT4 GetDirectionVector() const { return this->_directionVector; }
    
private:
    DX::XMFLOAT4 _camPosition = {0.0f, 0.0f, -40.0f, 1.0f};
    DX::XMFLOAT4 _directionVector = {-1.0f, 0.0f, 0.0f, 1.0f};
};
