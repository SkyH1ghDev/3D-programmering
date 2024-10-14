#pragma once

#include "SubMeshInfo.hpp"
#include "VertexInfo.hpp"

#include <vector>

namespace DX = DirectX;

// TODO: Refactor away this class. Unnecessary abstraction that makes implementation more bug prone.

class MeshData
{
public:
    VertexInfo VertexInfo;
    std::vector<SubMeshInfo> SubMeshInfoList;
    DX::XMFLOAT4 MeshPosition;

    DX::XMFLOAT4 OscillationStartPosition;
    float OscillationPeriod;
    float OscillationTime;
    
    std::bitset<1> MeshFlags;
};
