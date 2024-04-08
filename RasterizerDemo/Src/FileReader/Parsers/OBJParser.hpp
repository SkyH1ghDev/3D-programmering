#pragma once

#include "IFileParser.hpp"

class OBJParser : public IFileParser
{
public:
    
    OBJParser() = default;
    ~OBJParser() override = default;

    int GetVerticesFromFile(const std::string& filename) const;
};