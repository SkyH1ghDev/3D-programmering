#pragma once

#include <vector>
#include <string>
#include "ConfigBase.hpp"

class FileConfig : public ConfigBase
{
public:
    const std::vector<std::string> FilenameList = {"lego.obj"};
};
