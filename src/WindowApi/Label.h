#pragma once
#include "../font_render/font_rendering.h"
#include "Window.h"

#include <hmath.h>
#include <string>

#include <vector>
#include <iostream>

namespace linked
{
class WindowDiv;
class Label
{
#define DEFAULT_FONTSIZE 30
#define FONT_QUALITY 512		// size of the font image
#define DEFAULT_TEXTCOLOR hm::vec4(0,0,0,1)
public:
	Label(const WindowDiv& div, unsigned char* text, int textLength, hm::vec2 position, hm::vec4 textColor, Font_ID font_id, float margin, float padding);
	Label(const WindowDiv& div, unsigned char* text, int textLength, hm::vec2 position, Font_ID font_id);
	Label(const WindowDiv& div, unsigned char* text, int textLength, Font_ID font_id);
	void render();
	static void cleanUp();

	bool defaultLineSpace;
	std::string m_string;
	Font_ID m_font_id;
private:
	int m_font_size;
	const WindowDiv& m_div;
	// Label properties
	hm::vec2 m_position;
	float m_margin, m_padding;
	// Text properties
	unsigned char* m_text;
	int m_textLength;

	float m_text_width;
	float lineSpace;
	int m_yAdvance;
	hm::vec4 m_textColor;
public:
	inline float getTextWidth() { 
		string text;
		text.data = m_text;
		text.length = m_textLength - 1;
		hm::vec2 pos(0,0);
		int num_rendered = render_text_get_info(m_font_id, text, pos);
		return pos.x;
	}
	inline void setLineSpace(float lineSpace) { this->lineSpace = lineSpace; }
	inline hm::vec2 getPosition()const{ return this->m_position; }
	inline void setPosition(hm::vec2 position){ this->m_position = position; }
	inline hm::vec4 getTextColor() const{ return this->m_textColor; }
	inline void setTextColor(hm::vec4 color) { this->m_textColor = color; }

	inline void setMargin(float margin){ this->m_margin = margin; }
	inline float getMargin() const{ return this->m_margin; }
	inline void setText(unsigned char* newText, int length){ this->m_text = newText; this->m_textLength = length; }
	inline void setTextLength(int length){ this->m_textLength = length; }
	inline unsigned char* getText() { return m_text; };
	inline void setString(std::string& str) { this->m_string = str; }
	inline int getYAdvance() { return this->m_yAdvance; }
};
}
