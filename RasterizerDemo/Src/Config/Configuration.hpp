#pragma once

#include "FileConfig.hpp"
#include "ProjectionMatrixConfig.hpp"
#include "WorldMatrixConfig.hpp"
#include "ViewMatrixConfig.hpp"

class Configuration
{
public:
    Configuration() = default;
    
    FileConfig& GetFileConfig();
    ProjectionMatrixConfig& GetProjectionMatrixConfig(); 
    WorldMatrixConfig& GetWorldMatrixConfig();
    ViewMatrixConfig& GetViewMatrixConfig();
    
private:
    FileConfig _fileConfig;
    ProjectionMatrixConfig _projectionMatrixConfig;
    WorldMatrixConfig _worldMatrixConfig;
    ViewMatrixConfig _viewMatrixConfig;
};