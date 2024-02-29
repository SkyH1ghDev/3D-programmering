#pragma once

#include <string>
#include <vector>
#include "Vertex.hpp"

class IFileParser
{
public:
    IFileParser() = default;
    virtual ~IFileParser() = default;
    
protected:
    virtual std::vector<std::vector<std::string>> ReadContentsOfFile(const std::string& filename) const = 0;
};
