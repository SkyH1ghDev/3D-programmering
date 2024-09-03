#pragma once

#include "ImageData.hpp"

class MTLData
{
public:
	MTLData() = default;
	
public:
    ImageData ambientTextureData {{100, 100, 100, 255}, 1, 1, 4};
	ImageData diffuseTextureData {{100, 100, 100, 255}, 1, 1, 4};
	ImageData specularTextureData {{100, 100, 100, 255}, 1, 1, 4};
	ImageData colourTextureData {{0, 150, 0, 255}, 1, 1, 4};
	float specularExponent = 10000.0f;
};
