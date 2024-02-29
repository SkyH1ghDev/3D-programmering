#pragma once

#include "OBJParser.hpp"
#include "MTLParser.hpp"
#include "..\Vertex\Vertex.hpp"
#include <memory>

class FileReader
{
public:
    // CONSTRUCTORS
    FileReader();
    FileReader(const FileReader& other);
    
    // FUNCTIONS
    int ReadFile(const std::string& filename) const;
    
    // DESTRUCTOR
    ~FileReader();
    
private:
    // VARIABLES
    std::unique_ptr<MTLParser> _mtlParserPtr;
    std::unique_ptr<OBJParser> _objParserPtr;

    // FUNCTIONS
    static std::string GetFileExtension(const std::string& filename);
};
