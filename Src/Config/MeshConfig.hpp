#pragma once

#include <vector>
#include <string>
#include "IConfigBase.hpp"

class MeshConfig : public IConfigBase
{
public:
    std::vector<std::string> GetFilenameList() const { return this->_filenameList; }
    
private:
    std::vector<std::string> _filenameList = {"cube.obj", "Floor.obj"};
};
