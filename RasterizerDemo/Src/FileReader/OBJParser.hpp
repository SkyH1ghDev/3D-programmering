#pragma once

#include "IFileParser.hpp"

class OBJParser : public IFileParser
{
public:
    /*
     * Returns a std::vector<std::vector<std::string>> with all tokens indexed by line number from a file
     */
    
    OBJParser() = default;
    ~OBJParser() override = default;

private:
    std::string filename;
    
    std::vector<std::vector<std::string>> ReadFile(const std::string& filename) const override;
};