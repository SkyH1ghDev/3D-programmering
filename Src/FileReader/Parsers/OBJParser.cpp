#include "OBJParser.hpp"
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
                
               meshData.SubMeshInfoList.at(nrOfSubMeshes - 1).NrOfIndicesInSubMesh = vertexVector.size() - previousSubMeshInfo.StartIndexValue;
            }
            
            materialName = line.at(1);
            
            MTLParser mtlParser;
            mtlParser.GetMaterialFromFile(materialFileName, materialName, mtlData);
            
            SubMeshInfo subMeshInfo;
            subMeshInfo.StartIndexValue = vertexVector.size();
            subMeshInfo.MTLData = mtlData;

            // "TEMPORARY" Solution for textures
            ImageReader imageReader;
            subMeshInfo.MTLData.colourTextureData = imageReader.GetImageData("Pic.jpg"); // Get default image of dog

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
                //std::cout << "positionIndexVector at " << i << " = " << positionIndexVector.at(i) << "\n";
                //std::cout << "normalIndexVector at " << i << " = " << normalIndexVector.at(i) << "\n";
                //std::cout << "uvIndexVector at " << i << " = " << uvIndexVector.at(i) << "\n";

                int positionIndex = positionIndexVector.at(i);
                int normalIndex = normalIndexVector.at(i);
                int uvIndex = uvIndexVector.at(i);
                
                vertexVector.push_back(Vertex(positionVector.at(positionIndex), normalVector.at(normalIndex), uvVector.at(uvIndex)));
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
    meshData.SubMeshInfoList.back().NrOfIndicesInSubMesh = vertexVector.size() - lastSubMeshInfo.StartIndexValue; 
    return 0;
}
