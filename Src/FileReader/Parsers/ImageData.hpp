#pragma once

#include <vector>

class ImageData
{
public:
    ImageData() = default;
    ImageData(std::vector<unsigned char> textureData, size_t height, size_t width, int rgbaChannels)
    {
        this->TextureData = textureData;
        this->Height = height;
        this->Width = width;
        this->RGBAChannels = rgbaChannels;
    }
    
public:
    std::vector<unsigned char> TextureData;
    size_t Height;
    size_t Width;
    int RGBAChannels;
};