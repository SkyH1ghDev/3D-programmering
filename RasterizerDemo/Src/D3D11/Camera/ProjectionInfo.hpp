#pragma once

#include "ProjectionMatrixConfig.hpp"

class ProjectionInfo
{
public:
	float FovAngleY;
	float AspectRatio;
	float NearZ;
	float FarZ;

	ProjectionInfo(const ProjectionMatrixConfig& config);
	ProjectionInfo();
};

