#pragma once

#include "MTLData.hpp"

class SubMeshInfo
{
public:
	SubMeshInfo() = default;
	
public:
	UINT StartIndexValue = 0;
	UINT NrOfIndicesInSubMesh = 0;
	MTLData MTLData;
};
