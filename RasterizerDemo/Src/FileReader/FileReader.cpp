#include "FileReader.hpp"
#include <iostream>

#include "..\Helpers\D3D11\D3D11Helper.hpp"

// CONSTRUCTORS 
FileReader::FileReader()
{
    _mtlParserPtr = std::make_unique<MTLParser>();
    _objParserPtr = std::make_unique<OBJParser>();
}

FileReader::FileReader(const FileReader& other)
{
    // Temporarily empty
}

// FUNCTIONS
int FileReader::ReadFile(const std::string& filename) const
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

    return ("");
}


// DECONSTRUCTOR
FileReader::~FileReader() = default;
