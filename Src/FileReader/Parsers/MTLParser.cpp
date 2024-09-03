#include "MTLParser.hpp"

#include <bitset>

#include "ImageReader.hpp"

int MTLParser::GetMaterialFromFile(const std::string& filename, const std::string &materialName, MTLData &mtlData) const
{
    enum mtlFlags
    {
        correctMaterialFlag = 0,
        diffuseTexFlag = 1,
        specularTexFlag = 2,
        ambientTexFlag = 3,
        specularExpFlag = 4
    };
    
    std::vector<std::vector<std::string>> tokenVector = ReadContentsOfFile(filename);

    std::bitset<5> mtlBitset;
    mtlBitset.reset(); // Default all bits to 0
    
    for (auto line : tokenVector)
    {
        // If all bits are set, return
        if (mtlBitset.all())
        {
            return 0;
        }
        
        if (line.empty())
        {
            continue;
        }

        if (line.front() == "newmtl" && line.at(1) == materialName)
        {
            mtlBitset.set(correctMaterialFlag);
            continue;
        }

        // Barrier that continues if we haven't reached the correct material yet
        if (mtlBitset[correctMaterialFlag] == false)
        {
            continue;
        }

        if (line.front() == "Kd")
        {
            mtlBitset.set(diffuseTexFlag);
            
            mtlData.diffuseTextureData.TextureData = { static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                       static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                       static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                       255 };

            mtlData.diffuseTextureData.Height = 1;
            mtlData.diffuseTextureData.Width = 1;
            mtlData.diffuseTextureData.RGBAChannels = 4;

            continue;
        }

        if (line.front() == "map_Kd")
        {
            mtlBitset.set(diffuseTexFlag);
            
            mtlData.diffuseTextureData = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ks")
        {
            mtlBitset.set(specularTexFlag);
            
            mtlData.specularTextureData.TextureData = { static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                        static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                        static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                        255 };

            mtlData.specularTextureData.Height = 1;
            mtlData.specularTextureData.Width = 1;
            mtlData.specularTextureData.RGBAChannels = 4;

            continue;
        }

        if (line.front() == "map_Ks")
        {
            mtlBitset.set(specularTexFlag);
            
            mtlData.specularTextureData = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ka")
        {
            mtlBitset.set(ambientTexFlag);
            
            mtlData.ambientTextureData.TextureData = { static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                       static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                       static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                       255 };

            mtlData.ambientTextureData.Height = 1;
            mtlData.ambientTextureData.Width = 1;
            mtlData.ambientTextureData.RGBAChannels = 4;
            continue;
        }

        if (line.front() == "map_Ka")
        {
            mtlBitset.set(ambientTexFlag);
            
            mtlData.ambientTextureData = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ns")
        {
            mtlBitset.set(specularExpFlag);
            
            mtlData.specularExponent = std::stof(line.at(1));
            continue;
        }
    }
    
    return 0;
}