#pragma once

#include "SubMeshInfo.hpp"
#include "IndexInfo.hpp"
#include "VertexInfo.hpp"

#include <array>
#include <vector>

// TODO: Refactor away this class. Unnecessary abstraction that makes implementation more bug prone.

class MeshData
{
public:
    VertexInfo VertexInfo;
    std::vector<SubMeshInfo> SubMeshInfoList;
    std::array<float, 4> meshPosition;

    std::array<float, 4> oscillationPosition;
    float oscillationPeriod;
    float oscillationTime;
    
    std::bitset<1> meshFlags;
};
