#include "IFileParser.hpp"
#include <string>
#include <sstream>
#include <fstream>

class OBJParser : public IFileParser
{
public: 
    static void IFileParser::ReadFile(std::string filename)
    {
        std::fstream inFile;
        inFile.open(filename);

        std::string line;
        while(std::getline(inFile, line))
        {
            
        }

        inFile.close();
    }

    OBJParser();
    ~OBJParser();
    
private:
    
};