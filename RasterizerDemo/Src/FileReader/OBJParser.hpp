#pragma once

#include "IFileParser.hpp"

class OBJParser : public IFileParser
{
public:
   
    
    OBJParser() = default;
    ~OBJParser() override = default;

    friend class FileReader;
private:
    // VARIABLES
    std::string filename;

    // FUNCTIONS
    
    // Returns a std::vector<std::vector<std::string>> with all tokens indexed by line number from a file
    std::vector<std::vector<std::string>> ReadContentsOfFile(const std::string& filename) const override;

    //
    std::vector<Vertex> GetVerticesFromFile(const std::string& filename) const;
};