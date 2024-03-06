#pragma once

#include "MaterialManager.hpp"
#include "VertexManager.hpp"

class ManagerHelper
{
public:
    
    static void ReleaseManagers()
    {
        MaterialManager::ReleaseInstance();
        VertexManager::ReleaseInstance();
    }

private:

    ManagerHelper() = delete; 
    ManagerHelper(const ManagerHelper&) = delete;
    ManagerHelper& operator=(const ManagerHelper&) = delete;
};
