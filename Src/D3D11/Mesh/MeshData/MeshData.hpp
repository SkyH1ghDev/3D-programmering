#pragma once

#include "SubMeshInfo.hpp"
#include "IndexInfo.hpp"
#include "VertexInfo.hpp"

#include <vector>

class MeshData
{
public:
    VertexInfo VertexInfo;
    std::vector<SubMeshInfo> SubMeshInfoList;
};
