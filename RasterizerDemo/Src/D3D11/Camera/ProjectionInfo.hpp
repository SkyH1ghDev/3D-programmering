#pragma once

#include "ProjectionMatrixConfig.hpp"

class ProjectionInfo
{
public:
	ProjectionInfo(const ProjectionMatrixConfig& config);
	ProjectionInfo();
	
public:
	float FovAngleY;
	float AspectRatio;
	float NearZ;
	float FarZ;
};

