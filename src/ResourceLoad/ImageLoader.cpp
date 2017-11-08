#define STB_IMAGE_IMPLEMENTATION

#include "ImageLoader.h"
#include "stb_image.h"

ImageLoader::ImageLoader(std::string& filename, int nChannels)
{
	stbi_set_flip_vertically_on_load(1);
	loadedImage = stbi_load(filename.c_str(), &width, &height, &channels, nChannels);
}

ImageLoader::ImageLoader()
{
	loadedImage = NULL;
}

ImageLoader::~ImageLoader()
{
	//if (loadedImage) stbi_image_free(loadedImage);
}