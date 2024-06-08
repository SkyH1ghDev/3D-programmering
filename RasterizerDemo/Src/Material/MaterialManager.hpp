#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "MaterialData.hpp"

class MaterialManager 
{
public:
    
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
    
public:
    
    std::unordered_map<std::string, std::unique_ptr<MaterialData>> MaterialDataMap;

private:
    
    static MaterialManager* instance;
};


