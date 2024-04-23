#include "FileReader.hpp"
#include <iostream>

FileReader::FileReader()
{
    _mtlParserPtr = std::make_unique<MTLParser>();
    _objParserPtr = std::make_unique<OBJParser>();
}

int FileReader::ReadFilesFromConfig()
{
    FileConfig fileConfig;
    
    for(std::string file : fileConfig.GetFilenameList())
    {
        if (ReadFile(file) == -1)
        {
            std::cerr << "Could not read file: \"" << file << "\" \n";
            
            return -1;
        }
    }
    
    return 0;
}


int FileReader::ReadFile(const std::string& filename)
{
    const std::string fileExtension = GetFileExtension(filename);
    
    if (fileExtension == "obj")
    {
        if (_objParserPtr->GetVerticesFromFile(filename) == -1)
        {
            return -1;
        }
        return 0;
    }
    else if (fileExtension == "mtl")
    {
        if (_mtlParserPtr->GetMaterialFromFile(filename) == -1)
        {
            return -1;
        }
        return 0;
    }
    else
    {
        std::cerr << "Invalid file-type" << "\n";
        return -1;
    }
    
    return 0;
}

std::string FileReader::GetFileExtension(const std::string& filename)
{
    const size_t i = filename.rfind('.', filename.length());
    if (i != std::string::npos)
    {
        return(filename.substr(i+1, filename.length() - i));
    }

    return "";
}


// DECONSTRUCTOR
FileReader::~FileReader() = default;
