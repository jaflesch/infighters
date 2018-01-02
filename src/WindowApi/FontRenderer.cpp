#include "FontRenderer.h"
#include "FontShader.h"
#include "Mesh.h"
#include "../ResourceLoad/Texture.h"
#include "Primitive.h"
#include "../font_render/os.h"
#include "../font_render/font_rendering.h"

extern Window_Info window_info;

FontRenderer::FontRenderer(float fontSize, int textureQuality)
{
	this->fontSize = fontSize;
	fontMesh = new Mesh(new Quad(hm::vec3(0, 0, 0), 0, 0), true);
}

FontRenderer::~FontRenderer()
{
	delete fontMesh;
}

TextInfo FontRenderer::RenderText(const char* text, int length, float xPos, float yPos, float pixelWidthLimit, const hm::vec4 color, FontShader* shader, bool wordFormat)
{
	TextInfo info;
	
	hm::vec2 start_pos = hm::vec2((xPos / 2.0f) + 800, (yPos / 2.0f) + 450);
	info.start_position = start_pos;
	if (length > 0) {
		render_text(2, (u8*)text, length, start_pos, color);
	}
	info.width = xPos - start_pos.x;
	info.height = start_pos.y - yPos;
	info.end_position = start_pos;
	info.num_rows = 1;

	font_rendering_flush();
	return info;
}

hm::vec2 FontRenderer::RenderTextGetInfo(const char* text, int length) {
	hm::vec2 pos(0,0);
	string s;
	s.data = (u8*)text;
	s.length = length;
	render_text_get_info(2, s, pos);
	return pos;
}