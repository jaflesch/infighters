#pragma once
#include <string>
//#include <glm/glm.hpp>
#include <hmath.h>
#include "..\ho_gl.h"

struct ImageLoader
{
	ImageLoader(std::string& filename, int nChannels);
	ImageLoader();
	~ImageLoader();

	unsigned char* loadedImage;
	int width;
	int height;
	int channels;

};
