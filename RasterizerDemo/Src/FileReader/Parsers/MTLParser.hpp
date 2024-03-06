#pragma once

#include "IFileParser.hpp"

class MTLParser : public IFileParser
{
public:

    MTLParser() = default;
    ~MTLParser() override = default;
    
    friend class FileReader;
private:
    
    int GetMaterialFromFile(const std::string& filename) const;
};
