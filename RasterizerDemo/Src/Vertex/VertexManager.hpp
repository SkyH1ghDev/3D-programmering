#pragma once

#include "Vertex.hpp"
#include "Face.hpp"

class VertexManager
{
public:

    // VARIABLES

    std::vector<Face> FaceList;
    std::vector<std::array<float, 4>> NormalList;
    std::vector<std::array<float, 4>> PositionList;
    std::vector<std::array<float, 2>> UVList;
    
    //FUNCTIONS
    
    static VertexManager* GetInstance()
    {
        return instance == nullptr ? new VertexManager : instance;
    }

    static void ReleaseInstance()
    {
        delete instance;
    }


private:

    VertexManager();
    VertexManager(const VertexManager&) = delete;
    VertexManager& operator=(const VertexManager&) = delete;

    constexpr static VertexManager* instance = nullptr;
};
