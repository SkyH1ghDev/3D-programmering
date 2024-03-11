#pragma once

#include "Vertex.hpp"
#include "Face.hpp"
#include <memory>
#include <string>

class VertexGroupData;
class VertexMaterialData;

class VertexManager
{
public:

    // VARIABLES

    std::vector<std::unique_ptr<Face>> FaceList;
    std::vector<std::unique_ptr<VertexGroupData>> groupDataList;
    std::vector<std::unique_ptr<VertexMaterialData>> materialDataList;
    std::vector<std::string> materialLibraryList;
    std::vector<std::array<float, 4>> NormalList;
    std::vector<std::array<float, 4>> PositionList;
    std::vector<std::array<float, 2>> UVList;
    int numVerticesToDraw = 0;
    
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

class VertexGroupData
{
public:
    std::string groupName;
    int startIndex;
    int endIndex;
};

class VertexMaterialData
{
public:
    std::string materialName;
    int startIndex;
    int endIndex;
};