#pragma once

//#include "external/freetypegl/freetype-gl.h"
#include "..\freetypegl\freetype-gl.h"
#include <string>
#include <hmath.h>

class FontShader;
class Mesh;

struct TextInfo
{
	int num_rows;
	float width;
	float height;
	hm::vec2 start_position;
	hm::vec2 end_position;
};

class FontRenderer
{
public:
	FontRenderer(float fontSize, int textureQuality);
	~FontRenderer();
	TextInfo RenderText(std::string text, float xPos, float yPos, float pixelWidthLimit, const hm::vec4 color, FontShader* shader, bool wordFormat);
private:
	ftgl::texture_atlas_t* atlas;
	ftgl::texture_font_t* font;
	unsigned int* data;
	float fontSize;
	float ascender;
	Mesh* fontMesh;
public:
	inline float getFontSize() const { return this->fontSize; }
	inline float getAscender() const { return this->ascender; }
};

