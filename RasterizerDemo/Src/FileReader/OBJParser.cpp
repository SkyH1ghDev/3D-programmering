#include "OBJParser.hpp"
#include "VertexManager.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::vector<std::vector<std::string>> OBJParser::ReadContentsOfFile(const std::string& filename) const
{
    std::ifstream inFile;

    std::string filepath = "Resources/3D-Objects/" + filename;
    
    inFile.open(filepath);

    if (!inFile.good())
    {
        std::cerr << "Failed to open OBJ-file";
        exit(-1);
    }

    std::vector<std::vector<std::string>> lineVector;
    std::string line;

    // Gets line from file
    while(std::getline(inFile, line))
    {
        int start = 0;
        int end = 0;
        
        std::vector<std::string> tokenVector;
        
        // For each token in a line
        while ((start = line.find_first_not_of(' ', end)) != std::string::npos)
        {
            end = line.find(' ', start);

            tokenVector.emplace_back(line.substr(start, end - start));
        }

        lineVector.emplace_back(tokenVector);
    }

    inFile.close();
    return lineVector;
}

int OBJParser::GetVerticesFromFile(const std::string& filename) const
{
    std::vector<std::vector<std::string>> fileTokens = ReadContentsOfFile(filename);

    VertexManager* vertexManagerInstance = VertexManager::GetInstance();
    
    for (auto line : fileTokens)
    {
        if (line.front() == "v") // Add new vertex with positional information into vertex list
        {
            std::array<float, 4> positionArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 1.0f};
            vertexManagerInstance->PositionList.emplace_back(positionArray);
            continue;
        }

        if (line.front() == "vn")
        {
            std::array<float, 4> normalArray = { std::stof(line.at(1)), std::stof(line.at(2)), std::stof(line.at(3)), 0.0f };
            vertexManagerInstance->NormalList.emplace_back(normalArray);
        }

        // TODO: Add compatability for the "VertexPosIndex/VertexUVIndex" and "VertexPosIndex/VertexUVIndex/VertexNormalIndex" formats 
        if (line.front() == "f") 
        {
            bool first = true;

            vertexManagerInstance->FaceList.emplace_back(Face());
            
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
                        positionIndexVector.emplace_back(std::stoi(str));
                        continue;
                    case 2:
                        uvIndexVector.emplace_back(std::stoi(str));
                        continue;
                    case 3:
                        normalIndexVector.emplace_back(std::stoi(str));
                        continue;
                    default:
                        std::cerr << "Too many \"f\" arguments" << "\n";
                        return -1;
                    }
                }
            }           
        }
    }
    
    return 0;
}
