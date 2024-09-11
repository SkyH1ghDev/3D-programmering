﻿#include "FileReader.hpp"
#include <iostream>

FileReader::FileReader()
{
    _mtlParserPtr = std::make_unique<MTLParser>();
    _objParserPtr = std::make_unique<OBJParser>();
}

int FileReader::ReadFilesFromConfig(std::vector<MeshData> &meshDataList)
{
    MeshConfig meshConfig;

    MeshData meshData;

    for (size_t i = 0; i < meshConfig.GetNumFiles(); ++i)
    {
        const std::string& filename = meshConfig.GetFilenameAt(i);
        if (ReadFile(filename, meshData) == -1)
        {
            std::cerr << "Could not read file: \"" << filename << "\" \n";

            return -1;
        }

        std::array<float, 4> position = meshConfig.GetPositionAt(i);
        meshData.meshPosition = position;

        meshDataList.push_back(meshData);
    }
    
    return 0;
}


int FileReader::ReadFile(const std::string& filename, MeshData &meshData)
{
    const std::string fileExtension = GetFileExtension(filename);
    
    if (fileExtension == "obj")
    {
        if (this->_objParserPtr->GetVerticesFromFile(filename, meshData) == -1)
        {
            return -1;
        }
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
