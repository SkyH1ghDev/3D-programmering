#include "ImageReader.hpp"
#include "stb_image.h"
#include <vector>
#include <memory>

ImageData ImageReader::GetImageData(const std::string& filename) const
{
	int textureHeight, textureWidth, rgbaChannels;

	std::string imageDirectory = "Resources/Images/";
                	
	const std::unique_ptr<unsigned char[]> texelData(stbi_load((imageDirectory + filename).c_str(), &textureWidth, &textureHeight, &rgbaChannels, 0));
                
	rgbaChannels = 4;
		
	std::vector<unsigned char> textureData;
	textureData.resize(static_cast<size_t>(textureHeight) * static_cast<size_t>(textureWidth) * rgbaChannels); // The static cast gets rid of a warning :)
	
	int imageDataIndex = 0;
	for (int h = 0; h < textureHeight; ++h)
	{
		for (int w = 0; w < textureWidth; ++w)
		{
			const unsigned int pos0 = (w + textureWidth * h) * rgbaChannels;

			textureData[pos0 + 0] = texelData[imageDataIndex++];
			textureData[pos0 + 1] = texelData[imageDataIndex++];
			textureData[pos0 + 2] = texelData[imageDataIndex++];
			textureData[pos0 + 3] = 255;
				
		}
	}

	ImageData imageData;
	imageData.Height = textureHeight;
	imageData.Width = textureWidth;
	imageData.TextureData = textureData;
	imageData.RGBAChannels = rgbaChannels;
	
	return imageData;
}
