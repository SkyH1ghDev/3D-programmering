#pragma once

#include "IFileParser.hpp"

class MTLParser : public IFileParser
{
public:
    
    friend class FileReader;
    
private:
    
    std::vector<std::vector<std::string>> ReadContentsOfFile(const std::string& filename) const override;
};
