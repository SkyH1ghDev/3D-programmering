#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class MaterialData;

class MaterialManager 
{
public:

    // VARIABLES
    
    std::unordered_map<std::string, std::unique_ptr<MaterialData>> MaterialDataMap;
    
    // FUNCTIONS
    
    static MaterialManager* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new MaterialManager;
        }
        return instance;
    }

    static void ReleaseInstance()
    {
        delete instance;
    }

private:

    MaterialManager() = default; 
    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;
    
    static MaterialManager* instance;
};

class MaterialData
{
public:
    std::string materialName;
    std::vector<unsigned char> diffuseMap;
    std::vector<unsigned char> specularMap;
    std::vector<unsigned char> ambientMap;
    int specularExponent;
};
