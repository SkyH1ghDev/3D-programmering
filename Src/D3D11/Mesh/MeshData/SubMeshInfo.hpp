#pragma once

#include "MTLData.hpp"

class SubMeshInfo
{
public:
	SubMeshInfo() = default;
	
public:
	size_t StartIndexValue = 0;
	size_t NrOfIndicesInSubMesh = 0;
	MTLData MTLData;
};
