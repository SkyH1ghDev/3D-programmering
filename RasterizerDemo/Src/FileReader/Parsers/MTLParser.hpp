#pragma once

#include "IFileParser.hpp"
#include "ImageReader.hpp"

class MTLParser : public IFileParser
{
public:

    MTLParser() = default;
    ~MTLParser() override = default;
    
    int GetMaterialFromFile(const std::string& filename) const;

private:
    ImageReader imageReader;
};
