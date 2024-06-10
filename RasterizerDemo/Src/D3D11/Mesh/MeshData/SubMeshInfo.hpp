#pragma once
#include "ImageData.hpp"

class SubMeshInfo
{
public:
	SubMeshInfo() = default;
	
public:
	UINT StartIndexValue = 0;
	UINT NrOfIndicesInSubMesh = 0;
	ImageData AmbientTextureData;
	ImageData DiffuseTextureData;
	ImageData SpecularTextureData;
	float SpecularExponent;
};
