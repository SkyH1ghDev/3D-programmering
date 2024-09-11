#pragma once

#include "SubMeshInfo.hpp"
#include "IndexInfo.hpp"
#include "VertexInfo.hpp"

#include <array>
#include <vector>

class MeshData
{
public:
    VertexInfo VertexInfo;
    std::vector<SubMeshInfo> SubMeshInfoList;
    std::array<float, 4> meshPosition;
};
