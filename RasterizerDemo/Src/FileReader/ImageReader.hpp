#pragma once

#include <string>
#include <vector>

class ImageReader
{
public:
    std::vector<unsigned char> GetImageData(const std::string& filename) const;
};
