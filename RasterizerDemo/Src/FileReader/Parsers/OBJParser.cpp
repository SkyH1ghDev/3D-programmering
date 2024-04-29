#include "OBJParser.hpp"
#include "VertexManager.hpp"
#include "Mesh.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>

int OBJParser::GetVerticesFromFile(const std::string& filename) const
{
    std::vector<std::vector<std::string>> fileTokens = ReadContentsOfFile(filename);

    VertexManager* vertexManagerInstance = VertexManager::GetInstance();
    MeshData meshData;

    int faceIndex = 0;
    
    int groupIndexStart = 0;
    int materialIndexStart = 0;
    
    for (auto line : fileTokens)
    {
        if (line.empty())
        {
            continue;
        }

        if (line.front() == "g")
        {
            if (groupIndexStart > faceIndex)
            {
                continue;
            }
            
            std::unique_ptr<VertexGroupData> uGroupData = std::make_unique<VertexGroupData>();

            uGroupData->groupName = line.at(1);
            uGroupData->startIndex = groupIndexStart;
            uGroupData->endIndex = faceIndex;
            
            vertexManagerInstance->groupDataList.push_back(std::move(uGroupData));

            groupIndexStart = faceIndex + 1;
            continue;
        }

        if (line.front() == "mtllib")
        {
            vertexManagerInstance->materialLibraryList.push_back(line.at(1));
            continue;
        }

        if (line.front() == "usemtl")
        {
            if (materialIndexStart > faceIndex)
            {
                continue;
            }
            
            std::unique_ptr<VertexMaterialData> uMaterialData = std::make_unique<VertexMaterialData>();

            uMaterialData->materialName = line.at(1);
            uMaterialData->startIndex = materialIndexStart;
            uMaterialData->endIndex = faceIndex;

            vertexManagerInstance->materialDataList.push_back(std::move(uMaterialData));

            materialIndexStart = faceIndex + 1;
            continue;
        }
        
        if (line.front() == "v") // Add new vertex with positional information into vertex list
        {
            std::array<float, 4> positionArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 1.0f};
            vertexManagerInstance->PositionList.push_back(positionArray);
            continue;
        }

        if (line.front() == "vn")
        {
            std::array<float, 4> normalArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 0.0f };
            vertexManagerInstance->NormalList.push_back(normalArray);
            continue;
        }

        if (line.front() == "vt")
        {
            std::array<float, 2> uvArray = { std::stof(line.at(1)), std::stof(line.at(2)) };
            vertexManagerInstance->UVList.push_back(uvArray);
            continue;
        }

        if (line.front() == "f") 
        {
            bool first = true;

            std::vector<int> positionIndexVector;
            std::vector<int> uvIndexVector;
            std::vector<int> normalIndexVector;

            int numVertices = 0;
            
            for (auto string : line)
            {
                if (first) { first = false; continue; } // Skip first element of line

                ++numVertices;
                
                std::stringstream strStream(string);
                std::string str;
                
                int indexNumber = 0;
                
                while(getline(strStream, str, '/'))
                {
                    ++indexNumber;
                    switch (indexNumber)
                    {
                    case 1:
                        positionIndexVector.push_back(std::stoi(str) - 1);
                        continue;
                    case 2:
                        uvIndexVector.push_back(std::stoi(str) - 1);
                        continue;
                    case 3:
                        normalIndexVector.push_back(std::stoi(str) - 1);
                        continue;
                    default:
                        std::cerr << "Too many \"f\" arguments" << "\n";
                        return -1;
                    }
                }
            }

            // Check that there is at least one valid vertex, normal and uv that could be read from the file 
            {
                if (positionIndexVector.empty())
                {
                    std::cerr << "No Valid Vertex Positions Could Be Read" << "\n";
                    return -1;
                }

                if (uvIndexVector.empty())
                {
                    std::cerr << "No Valid Vertex UVs Could Be Read" << "\n";
                    return -1;
                }

                if (normalIndexVector.empty())
                {
                    std::cerr << "No Valid Vertex Normals Could Be Read" << "\n";
                    return -1;
                }
            }

            
            if (numVertices == 4) // Face is a quad
            {
                std::unique_ptr<Face> uFace1 = std::make_unique<Face>();
                std::unique_ptr<Face> uFace2 = std::make_unique<Face>();

                uFace1->AddVertexPositionIndex(std::vector<int>({positionIndexVector.at(0), positionIndexVector.at(1), positionIndexVector.at(2)}));
                uFace2->AddVertexPositionIndex(std::vector<int>({positionIndexVector.at(0), positionIndexVector.at(2), positionIndexVector.at(3)}));

                uFace1->AddUVIndex(std::vector<int>({uvIndexVector.at(0), uvIndexVector.at(1), uvIndexVector.at(2)}));
                uFace2->AddUVIndex(std::vector<int>({uvIndexVector.at(0), uvIndexVector.at(2), uvIndexVector.at(3)}));
                
                uFace1->AddNormalIndex(std::vector<int>({normalIndexVector.at(0), normalIndexVector.at(1), normalIndexVector.at(2)}));
                uFace2->AddNormalIndex(std::vector<int>({normalIndexVector.at(0), normalIndexVector.at(2), normalIndexVector.at(3)}));

                vertexManagerInstance->FaceList.push_back(std::move(uFace1));
                vertexManagerInstance->FaceList.push_back(std::move(uFace2));

                faceIndex += 2;
            }
            else
            {
                ++faceIndex;
                        
                std::unique_ptr<Face> uFace = std::make_unique<Face>();
                
                uFace->AddVertexPositionIndex(positionIndexVector);
                uFace->AddUVIndex(uvIndexVector);
                uFace->AddNormalIndex(normalIndexVector);
    
                vertexManagerInstance->FaceList.push_back(std::move(uFace));    
            }
        }
    }

    for (auto &face : vertexManagerInstance->FaceList)
    {
        for (auto vertex : face->VertexPositionIndices())
        {
            vertexManagerInstance->numVerticesToDraw++;
        }
    }
    
    return 0;
}


