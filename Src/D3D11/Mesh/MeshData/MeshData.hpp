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
    std::array<float, 4> MeshPosition;

    std::array<float, 4> OscillationStartPosition;
    float OscillationPeriod;
    float OscillationTime;
    
    std::bitset<1> MeshFlags;
};
