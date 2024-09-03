#pragma once

#include "Vertex.hpp"

class VertexInfo
{
public:
    VertexInfo() = default;
    
public:
    UINT SizeOfVertex = sizeof(Vertex);
    size_t NrOfVerticesInBuffer;
    std::vector<Vertex> VertexVector;
};
