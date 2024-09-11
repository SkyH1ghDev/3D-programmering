#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <array>
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
    
private:
    std::vector<std::string> _filenameList = {"cube.obj", "Floor.obj"};
    std::vector<std::array<float, 4>> _positionList = {{0.0f, 10.0f, 0.0f, 1.0f}, {0.0f, -10.0f, 0.0f, 1.0f}};
    
};
