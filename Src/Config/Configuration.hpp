#pragma once

#include <memory>

#include "FileConfig.hpp"
#include "ProjectionMatrixConfig.hpp"
#include "WorldMatrixConfig.hpp"
#include "ViewMatrixConfig.hpp"
#include "WindowConfig.hpp"
#include "RenderConfig.hpp"

class Configuration
{
public:
    Configuration() = default;
    Configuration operator=(const Configuration& other) = delete;
    Configuration(const Configuration& other) = delete;
    
    FileConfig GetFileConfig() const { return this->_fileConfig; }
    ProjectionMatrixConfig GetProjectionMatrixConfig() const { return this->_projectionMatrixConfig; }
    WorldMatrixConfig GetWorldMatrixConfig() const { return this->_worldMatrixConfig; }
    ViewMatrixConfig GetViewMatrixConfig() const { return this->_viewMatrixConfig; }
    WindowConfig GetWindowConfig() const { return this->_windowConfig; }
    RenderConfig GetRenderConfig() const { return this->_renderConfig; }
    
private:
    FileConfig _fileConfig;
    ProjectionMatrixConfig _projectionMatrixConfig;
    WorldMatrixConfig _worldMatrixConfig;
    ViewMatrixConfig _viewMatrixConfig;
    WindowConfig _windowConfig;
    RenderConfig _renderConfig;
};