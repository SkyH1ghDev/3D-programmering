#pragma once

#include <string>
#include "ImageData.hpp"

class ImageReader
{
public:
    ImageData GetImageData(const std::string& filename) const;
};
