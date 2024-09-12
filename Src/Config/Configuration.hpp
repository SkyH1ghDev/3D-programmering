#pragma once

#include <memory>

#include "MeshConfig.hpp"
#include "ProjectionMatrixConfig.hpp"
#include "ViewMatrixConfig.hpp"
#include "WindowConfig.hpp"
#include "RenderConfig.hpp"

class Configuration
{
public:
    Configuration() = default;
    Configuration operator=(const Configuration& other) = delete;
    Configuration(const Configuration& other) = delete;
    
    MeshConfig GetFileConfig() const { return this->_fileConfig; }
    ProjectionMatrixConfig GetProjectionMatrixConfig() const { return this->_projectionMatrixConfig; }
    ViewMatrixConfig GetViewMatrixConfig() const { return this->_viewMatrixConfig; }
    WindowConfig GetWindowConfig() const { return this->_windowConfig; }
    RenderConfig GetRenderConfig() const { return this->_renderConfig; }
    
private:
    MeshConfig _fileConfig;
    ProjectionMatrixConfig _projectionMatrixConfig;
    ViewMatrixConfig _viewMatrixConfig;
    WindowConfig _windowConfig;
    RenderConfig _renderConfig;
};