#pragma once

#include "ConfigBase.hpp"
#include "WindowConfig.hpp"

class ProjectionMatrixConfig : public ConfigBase
{
private:
	WindowConfig _windowConfig;
	
public:
	const float AspectRatio = static_cast<float>(this->_windowConfig.Width) / static_cast<float>(this->_windowConfig.Height);
	const float FovAngle = 59.0f; // Degrees
	const float NearZ = 0.1f;
	const float FarZ = 100.0f;
};
