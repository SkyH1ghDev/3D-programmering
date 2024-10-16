#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <bitset>

#include <DirectXMath.h>

#include "IConfigBase.hpp"

namespace DX = DirectX;

class MeshConfig : public IConfigBase
{
public:

    MeshConfig()
    {
        assert(this->_filenameList.size() == this->_positionList.size());
    }
    
    size_t GetNumFiles() const { return this->_filenameList.size(); }
    std::string GetFilenameAt(size_t idx) const { return this->_filenameList.at(idx); }
    DX::XMFLOAT4 GetPositionAt(size_t idx) const { return this->_positionList.at(idx); }
    
    std::bitset<1> GetMeshFlagsAt(size_t idx) const { return this->_meshFlagList.at(idx); }

    DX::XMFLOAT4 GetOscillationPositionAt(size_t idx) const { return this->_oscillationPositionList.at(idx); }
    float GetOscillationPeriodAt(size_t idx) const { return this->_periodList.at(idx); }
    float GetOscillationTimeAt(size_t idx) const { return this->_timeList.at(idx); } 
    
private:
    std::vector<std::string> _filenameList = {"cube.obj", "Floor.obj", "suzanne.obj", "suzanne.obj", "suzanne.obj",
                                                "suzanne.obj", "cube.obj", "cube.obj", "cube.obj", "cube.obj"};
    
    std::vector<DX::XMFLOAT4> _positionList = {{-2.0f, -7.0f, -7.0f, 1.0f}, {0.0f, -10.0f, 0.0f, 1.0f}, {-5.0f, 5.0f, 5.0f, 1.0f}, {6.0f, 20.0f, 0.0f, 1.0f}, {12.0f, 17.0f, 13.0f, 1.0f},
                                                  {-10.0f, 15.0f, -10.0f, 1.0f}, {-15.0f, 20.0f, -15.0f, 1.0f}, {30.0f, 5.0f, 0.0f, 1.0f}, {0.0f, 7.0f, 25.0f, 1.0f}, {-17.0f, 23.0f, 23.0f, 1.0f}};

    // bit 1 : Oscillation
    std::vector<std::bitset<1>> _meshFlagList = {{1}, {0}, {0}, {0}, {0},
                                                    {0}, {0}, {0}, {0}, {0}};

    std::vector<DX::XMFLOAT4> _oscillationPositionList = {{2.0f, -7.0f, -7.0f, 1.0f}, DX::XMFLOAT4(), DX::XMFLOAT4(), DX::XMFLOAT4(), DX::XMFLOAT4(),
                                                            DX::XMFLOAT4(), DX::XMFLOAT4(), DX::XMFLOAT4(), DX::XMFLOAT4(), DX::XMFLOAT4()};
    
    std::vector<float> _periodList = {10.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    
    std::vector<float> _timeList = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};
