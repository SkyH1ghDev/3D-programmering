#include "Configuration.hpp"

FileConfig& Configuration::GetFileConfig() { return this->_fileConfig; }

ProjectionMatrixConfig& Configuration::GetProjectionMatrixConfig() { return this->_projectionMatrixConfig; }

WorldMatrixConfig& Configuration::GetWorldMatrixConfig() { return this->_worldMatrixConfig; }

ViewMatrixConfig& Configuration::GetViewMatrixConfig() { return this->_viewMatrixConfig; }