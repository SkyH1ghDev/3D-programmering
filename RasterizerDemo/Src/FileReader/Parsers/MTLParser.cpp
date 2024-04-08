#include "MTLParser.hpp"
#include "MaterialManager.hpp"
#include "ImageReader.hpp"

int MTLParser::GetMaterialFromFile(const std::string& filename) const
{
    std::vector<std::vector<std::string>> tokenVector = ReadContentsOfFile(filename);

    MaterialManager* materialManagerInstance = MaterialManager::GetInstance();

    MaterialData* currentMaterialData = nullptr;

    int currentMaterialIndex = 0;
    
    for (auto line : tokenVector)
    {
        if (line.empty())
        {
            continue;
        }

        if (line.front() == "newmtl")
        {
            std::unique_ptr<MaterialData> uMaterialData = std::make_unique<MaterialData>();
            currentMaterialData = uMaterialData.get();
            
            uMaterialData->materialName = line.at(1);

            std::pair<std::string, std::unique_ptr<MaterialData>> pair = std::make_pair(uMaterialData->materialName, std::move(uMaterialData));
            
            materialManagerInstance->MaterialDataMap.insert(std::move(pair));
            ++currentMaterialIndex;
            continue;
        }

        if (line.front() == "Kd")
        {
            currentMaterialData->diffuseMap = std::vector<unsigned char>({  static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                                                static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                                                static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                                                255});
            continue;
        }

        if (line.front() == "map_Kd")
        {
            currentMaterialData->diffuseMap = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ks")
        {
            currentMaterialData->specularMap = std::vector<unsigned char>({ static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                                                static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                                                static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                                                255});
            continue;
        }

        if (line.front() == "map_Ks")
        {
            currentMaterialData->specularMap = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ka")
        {
             currentMaterialData->ambientMap = std::vector<unsigned char>({ static_cast<unsigned char>(round(255 * std::stof(line.at(1)))),
                                                                                 static_cast<unsigned char>(round(255 * std::stof(line.at(2)))),
                                                                                 static_cast<unsigned char>(round(255 * std::stof(line.at(3)))),
                                                                                 255});
            continue;
        }

        if (line.front() == "map_Ka")
        {
            currentMaterialData->ambientMap = imageReader.GetImageData(line.at(1));
            continue;
        }

        if (line.front() == "Ns")
        {
            currentMaterialData->specularExponent = std::stoi(line.at(1));
            continue;
        }
    }
    
    return 0;
}