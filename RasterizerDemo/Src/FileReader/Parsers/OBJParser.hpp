#pragma once

#include "IFileParser.hpp"
#include "Mesh.hpp"

class OBJParser : public IFileParser
{
public:
    
    OBJParser() = default;
    ~OBJParser() override = default;

    int GetVerticesFromFile(const std::string& filename, MeshData &meshData) const;
};
