#pragma once
#include "ImageData.hpp"

class SubMeshInfo
{
public:
	SubMeshInfo() = default;
	
public:
	UINT startIndexValue = 0;
	UINT nrOfIndicesInSubMesh = 0;
	ImageData AmbientTextureData;
	ImageData DiffuseTextureData;
	ImageData SpecularTextureData;
	float specularExponent;
};
