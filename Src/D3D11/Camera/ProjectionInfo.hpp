#pragma once

#include "ProjectionMatrixConfig.hpp"

class ProjectionInfo
{
public:
	ProjectionInfo(const ProjectionMatrixConfig& config);
	ProjectionInfo();
	ProjectionInfo(const ProjectionInfo& other) = default;
	ProjectionInfo& operator=(const ProjectionInfo& other) = default;
	
public:
	float FovAngleY;
	float AspectRatio;
	float NearZ;
	float FarZ;
};

