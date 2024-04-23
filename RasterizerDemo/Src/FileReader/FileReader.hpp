#pragma once

#include "OBJParser.hpp"
#include "MTLParser.hpp"
#include "FileConfig.hpp"
#include <memory>

class FileReader
{
public:
    FileReader();
    
    int ReadFilesFromConfig();
    ~FileReader();
    
private:
    // VARIABLES
    std::unique_ptr<MTLParser> _mtlParserPtr;
    std::unique_ptr<OBJParser> _objParserPtr;

    // FUNCTIONS
    std::string GetFileExtension(const std::string& filename);
    int ReadFile(const std::string& filename);
};
