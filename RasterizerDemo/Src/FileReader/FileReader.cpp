#include "FileReader.hpp"
#include <iostream>

#include "D3D11Helper.h"

// CONSTRUCTORS 
FileReader::FileReader()
{
    _matParserPtr = std::make_unique<MTLParser>();
    _objParserPtr = std::make_unique<OBJParser>();
}

FileReader::FileReader(const FileReader& other)
{
    // Temporarily empty
}

// FUNCTIONS
std::vector<Vertex> FileReader::ReadFile(const std::string& filename) const
{
    const std::string fileExtension = GetFileExtension(filename);
    std::vector<Vertex> vertices; 
    
    switch(fileExtension)
    {
    case "obj":
        vertices = _objParserPtr->GetVerticesFromFile(filename);
        break;
        
    case "mat":
        _matParserPtr->ReadContentsOfFile(filename);
        break;
        
    default:
        std::cerr << "Invalid file-type" << "\n";
        break;
    }
    
    return vertices;
}

std::string FileReader::GetFileExtension(const std::string& filename)
{
    const size_t i = filename.rfind('.', filename.length());
    if (i != std::string::npos)
    {
        return(filename.substr(i+1, filename.length() - i));
    }

    return ("");
}


// DECONSTRUCTOR
FileReader::~FileReader() = default;
