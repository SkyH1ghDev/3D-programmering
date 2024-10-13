#pragma once

#include <vector>
#include <DirectXMath.h>

#include "IConfigBase.hpp"
namespace DX = DirectX;

class ViewMatrixConfig : public IConfigBase
{
public:
    const DX::XMFLOAT4& GetCamPositionAt(const size_t& index) const { return this->_camPositions.at(index); }
    
    
private:
    std::vector<DX::XMFLOAT4> _camPositions = {{0.0f, 0.0f, -7.0f, 1.0f},
                                               {0.0f, 0.0f, 7.0f, 1.0f}};
};
