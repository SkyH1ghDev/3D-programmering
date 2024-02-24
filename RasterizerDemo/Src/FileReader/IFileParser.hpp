#pragma once

#include <string>
#include <vector>

class IFileParser
{
public:
    IFileParser() = default;
    virtual ~IFileParser() = default;
    
protected:
    virtual 
    virtual std::vector<std::vector<std::string>> ReadFile(const std::string& filename) const = 0;
};
