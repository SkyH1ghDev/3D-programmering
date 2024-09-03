#pragma once

#include "IFileParser.hpp"
#include "ImageReader.hpp"
#include "MTLData.hpp"

class MTLParser : public IFileParser
{
public:

    MTLParser() = default;
    ~MTLParser() override = default;
    
    int GetMaterialFromFile(const std::string& filename, const std::string &materialName, MTLData &mtlData) const;

private:
    ImageReader imageReader;
};
