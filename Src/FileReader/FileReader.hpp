#pragma once

#include "OBJParser.hpp"
#include "MTLParser.hpp"
#include "MeshConfig.hpp"
#include <memory>

class FileReader
{
public:
    
    FileReader();
    
    int ReadFilesFromConfig(std::vector<MeshData> &meshDataList);
    int ReadFileFromFilename(MeshData& meshData, const std::string& filename);
    ~FileReader();
    
private:
    std::string GetFileExtension(const std::string& filename);
    int ReadFile(const std::string& filename, MeshData &meshData);
    
private:
    std::unique_ptr<MTLParser> _mtlParserPtr;
    std::unique_ptr<OBJParser> _objParserPtr;
};
