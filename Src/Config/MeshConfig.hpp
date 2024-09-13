#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <array>
#include <bitset>

#include "IConfigBase.hpp"

class MeshConfig : public IConfigBase
{
public:

    MeshConfig()
    {
        assert(this->_filenameList.size() == this->_positionList.size());
    }
    
    size_t GetNumFiles() const { return this->_filenameList.size(); }
    std::string GetFilenameAt(size_t idx) const { return this->_filenameList.at(idx); }
    std::array<float, 4> GetPositionAt(size_t idx) const { return this->_positionList.at(idx); }
    
    std::bitset<1> GetMeshFlagsAt(size_t idx) const { return this->_meshFlagList.at(idx); }

    std::array<float, 4> GetOscillationPositionAt(size_t idx) const { return this->_oscillationPositionList.at(idx); }
    float GetOscillationPeriodAt(size_t idx) const { return this->_periodList.at(idx); }
    float GetOscillationTimeAt(size_t idx) const { return this->_timeList.at(idx); } 
    
private:
    std::vector<std::string> _filenameList = {"cube.obj", "Floor.obj"};
    std::vector<std::array<float, 4>> _positionList = {{0.0f, 10.0f, 0.0f, 1.0f}, {0.0f, -10.0f, 0.0f, 1.0f}};

    // bit 1 : Oscillation
    std::vector<std::bitset<1>> _meshFlagList = {{1}, {0}};

    std::vector<std::array<float, 4>> _oscillationPositionList = {{10.0f, 5.0f, 10.0f, 1.0f}, std::array<float, 4>()};
    std::vector<float> _periodList = {5.0f, 0.0f};
    std::vector<float> _timeList = {0.0f, 0.0f};
};
