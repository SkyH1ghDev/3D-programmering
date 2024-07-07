#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class IFileParser
{
public:
    IFileParser() = default;
    virtual ~IFileParser() = default;
    
protected:
    
    // Returns a std::vector<std::vector<std::string>> with all tokens indexed by line number from a file
    std::vector<std::vector<std::string>> ReadContentsOfFile(const std::string& filename) const
    {
        std::ifstream inFile;

        std::string filepath = "Resources/3D-Objects/" + filename;
        
        inFile.open(filepath);

        if (!inFile.good())
        {
            std::cerr << "Failed to open OBJ-file";
            exit(-1); //TODO: Remove this
        }

        std::vector<std::vector<std::string>> lineVector;
        std::string line;

        // Gets line from file
        while(std::getline(inFile, line))
        {
            size_t start = 0;
            size_t end = 0;
            
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
};
