#pragma once

#include "Vertex.hpp"

class VertexInfo
{
public:
    VertexInfo() = default;
    
public:
    UINT SizeOfVertex = sizeof(Vertex);
    UINT NrOfVerticesInBuffer;
    std::vector<Vertex> VertexVector;
};
