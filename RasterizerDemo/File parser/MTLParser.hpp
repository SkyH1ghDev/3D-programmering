#pragma once

#include "IFileParser.hpp"

class MTLParser : public IFileParser
{
public:
    /*
     * Returns a std::vector<std::vector<std::string>> with all tokens indexed by line number from a file
     */
    std::vector<std::vector<std::string>> ReadFile(const std::string& filename) const override;
    
    MTLParser() = default;
    ~MTLParser() override = default;
};