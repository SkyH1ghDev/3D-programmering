#include "ImageReader.hpp"
#include "stb_image.h"
#include <vector>
#include <memory>

std::vector<unsigned char> ImageReader::GetImageData(const std::string& filename) const
{
	int textureHeight, textureWidth, rgbaChannels;

	std::string imageDirectory = "Resources/Images/";
                	
	const std::unique_ptr<unsigned char[]> imageData(stbi_load((imageDirectory + filename).c_str(), &textureWidth, &textureHeight, &rgbaChannels, 0));
                
	rgbaChannels = 4;
		
	std::vector<unsigned char> textureData;
	textureData.resize(textureHeight * textureWidth * rgbaChannels);
	
	int imageDataIndex = 0;
	for (int h = 0; h < textureHeight; ++h)
	{
		for (int w = 0; w < textureWidth; ++w)
		{
			unsigned int pos0 = (w + textureWidth * h) * rgbaChannels;

			textureData[pos0 + 0] = imageData[imageDataIndex++];
			textureData[pos0 + 1] = imageData[imageDataIndex++];
			textureData[pos0 + 2] = imageData[imageDataIndex++];
			textureData[pos0 + 3] = 255;
				
		}
	}

	return textureData;
}
