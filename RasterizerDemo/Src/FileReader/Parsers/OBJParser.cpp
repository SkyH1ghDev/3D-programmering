#include "OBJParser.hpp"
#include "VertexManager.hpp"
#include "Mesh.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "MTLParser.hpp"


int OBJParser::GetVerticesFromFile(const std::string& filename, MeshData &meshData) const
{
    std::vector<std::vector<std::string>> fileTokens = ReadContentsOfFile(filename);

    std::vector<std::array<float, 2>> uvVector;
    std::vector<std::array<float, 4>> positionVector;
    std::vector<std::array<float, 4>> normalVector;
    std::vector<Vertex> vertexVector;
    
    UINT nrOfIndicesInSubMesh = 0;
    UINT nrOfSubMeshes = 0;
    std::string materialName;
    std::string materialFileName;
    MTLData mtlData;
    
    for (auto line : fileTokens)
    {
        if (line.empty())
        {
            continue;
        }

        if (line.front() == "mtllib")
        {
            materialFileName = line.at(1);
            
            continue;   
        }

        if (line.front() == "usemtl")
        {
            if (materialFileName.empty())
            {
                std::cerr << "No file given for the material \n";
                return -1;
            }

            // Set nr of indices for previous SubMeshInfo
            if (nrOfSubMeshes > 0)
            {
                SubMeshInfo previousSubMeshInfo = meshData.SubMeshInfoList.at(nrOfSubMeshes - 1);
                
               meshData.SubMeshInfoList.at(nrOfSubMeshes - 1).nrOfIndicesInSubMesh = vertexVector.size() - previousSubMeshInfo.startIndexValue;
            }
            
            materialName = line.at(1);
            
            MTLParser mtlParser;
            mtlParser.GetMaterialFromFile(materialFileName, materialName, mtlData);
            
            SubMeshInfo subMeshInfo;
            subMeshInfo.startIndexValue = vertexVector.size();
            subMeshInfo.AmbientTextureData = mtlData.ambientTextureData;
            subMeshInfo.DiffuseTextureData = mtlData.diffuseTextureData;
            subMeshInfo.SpecularTextureData = mtlData.specularTextureData;
            subMeshInfo.specularExponent = mtlData.specularExponent;

            meshData.SubMeshInfoList.push_back(subMeshInfo);
            ++nrOfSubMeshes;
            
            nrOfIndicesInSubMesh = 0;
            
            continue;
        }
        
        if (line.front() == "v") // Add new vertex with positional information into vertex list
        {
            std::array<float, 4> positionArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 1.0f};
            positionVector.push_back(positionArray);
            
            continue;
        }

        if (line.front() == "vn")
        {
            std::array<float, 4> normalArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 0.0f };
            normalVector.push_back(normalArray);
            
            continue;
        }

        if (line.front() == "vt")
        {
            std::array<float, 2> uvArray = { std::stof(line.at(1)), std::stof(line.at(2)) };
            uvVector.push_back(uvArray);
            
            continue;
        }

        if (line.front() == "f") 
        {
            bool first = true;
            
            std::vector<int> positionIndexVector;
            std::vector<int> uvIndexVector;
            std::vector<int> normalIndexVector;

            for (auto string : line)
            {
                if (first) { first = false; continue; } // Skip first element of line

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

            // Pushback 3 vertices (Triangle)

            for (int i = 0; i < 3; ++i)
            {
                vertexVector.push_back(Vertex(positionVector.at(i), normalVector.at(i), uvVector.at(i)));
                ++nrOfIndicesInSubMesh;
            }
            
            continue;
        }
    }

    VertexInfo vertexInfo;
    vertexInfo.NrOfVerticesInBuffer = vertexVector.size();
    vertexInfo.VertexVector = vertexVector; // Maybe goes out of scope IDK. Needs to be tested.
    
    meshData.VertexInfo = vertexInfo;

    // Set last SubMeshInfo nrOfIndicesInSubMesh
    SubMeshInfo lastSubMeshInfo = meshData.SubMeshInfoList.back();
    meshData.SubMeshInfoList.back().nrOfIndicesInSubMesh = vertexVector.size() - lastSubMeshInfo.startIndexValue;
    
    return 0;
}

// Deprecated code
/*
int OBJParser::GetVerticesFromFile(const std::string& filename) const
{
    std::vector<std::vector<std::string>> fileTokens = ReadContentsOfFile(filename);

    VertexManager* vertexManagerInstance = VertexManager::GetInstance();
    
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
} */


