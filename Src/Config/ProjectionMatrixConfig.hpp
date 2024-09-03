#pragma once

#include "IConfigBase.hpp"
#include "WindowConfig.hpp"

class ProjectionMatrixConfig : public IConfigBase
{
public:
	ProjectionMatrixConfig() : _aspectRatio(static_cast<float>(this->_windowConfig.GetWidth()) / static_cast<float>(this->_windowConfig.GetHeight())) {}

public:	
	float GetAspectRatio() const { return _aspectRatio; }
	float GetFovAngle() const { return _fovAngle; }
	float GetNearZ() const { return _nearZ; }
	float GetFarZ() const { return _farZ; }
	
private:
	WindowConfig _windowConfig;
	
	float _aspectRatio;
	float _fovAngle = 59.0f; // Degrees
	float _nearZ = 0.1f;
	float _farZ = 100.0f;
};
