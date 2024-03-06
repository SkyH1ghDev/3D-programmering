#pragma once

class MaterialManager 
{
public:

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
