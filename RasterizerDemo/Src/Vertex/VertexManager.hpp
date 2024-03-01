#pragma once

#include "Vertex.hpp"
#include "Face.hpp"
#include <memory>

class VertexManager
{
public:

    // VARIABLES

    std::vector<std::unique_ptr<Face>> FaceList;
    std::vector<std::array<float, 4>> NormalList;
    std::vector<std::array<float, 4>> PositionList;
    std::vector<std::array<float, 2>> UVList;
    
    //FUNCTIONS
    
    static VertexManager* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new VertexManager;
        }
        return instance;
    }

    static void ReleaseInstance()
    {
        delete instance;
    }


private:

    VertexManager() = default; 
    VertexManager(const VertexManager&) = delete;
    VertexManager& operator=(const VertexManager&) = delete;

    static VertexManager* instance;
};