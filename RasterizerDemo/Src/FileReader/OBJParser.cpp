#include "OBJParser.hpp"
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

            tokenVector.push_back(line.substr(start, end - start));
        }

        lineVector.push_back(tokenVector);
    }

    inFile.close();
    return lineVector;
}

std::vector<Vertex> OBJParser::GetVerticesFromFile(const std::string& filename) const
{
    std::vector<std::vector<std::string>> fileTokens = ReadContentsOfFile(filename);

    

    return std::vector<Vertex>();
}
