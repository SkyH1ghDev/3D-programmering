#pragma once

#include "IFileParser.hpp"

class OBJParser : public IFileParser
{
public:
    
    OBJParser() = default;
    ~OBJParser() override = default;

    friend class FileReader;
private:
    
    int GetVerticesFromFile(const std::string& filename) const;
};