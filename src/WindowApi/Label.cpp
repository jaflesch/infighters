#include "Label.h"
#include "WindowDiv.h"
#include "../font_render/os.h"

extern Window_Info window_info;

namespace linked
{
	/* LABELS */

	Label::Label(
		const WindowDiv& div, 
		unsigned char* text, 
		int textLength, 
		hm::vec2 position,
		hm::vec4 textColor, 
		Font_ID font_id,
		float margin, 
		float padding) :
		m_div(div),
		m_position(position), 
		m_margin(margin),
		m_padding(padding), 
		m_text(text), 
		m_font_id(font_id),
		m_textLength(textLength),
		m_textColor(textColor),
		defaultLineSpace(true),
		lineSpace(0), 
		m_yAdvance(0)
	{
		m_font_size = (int)get_font_size(font_id);
	}

	Label::Label(const WindowDiv& div, unsigned char* text, int textLength, hm::vec2 position, Font_ID font_id) :
		Label(div, text, textLength, position, DEFAULT_TEXTCOLOR, font_id, 0, 0){}

	Label::Label(const WindowDiv& div, unsigned char* text, int textLength, Font_ID font_id) :
		Label(div, text, textLength, hm::vec2(0, 0), DEFAULT_TEXTCOLOR, font_id, 0, 0){}

	void Label::cleanUp()
	{
	}

	void Label::render()
	{
		// Calculate the lineSpacing
		hm::vec2 renderPosition = m_div.getDivBasePosition(m_margin + m_position.x, m_margin + m_position.y + (lineSpace / 2.0f));

		float rightLimit = m_div.getWindow().getPosition().x * 2.0f + m_div.getWidth() - 50;

		int ww = window_info.width;
		int wh = window_info.height;

		if (m_text)
		{
			Font_ID font_id = this->m_font_id;
			if (font_id >= FONT_NUMBER) {
				font_id = 0;
			}
			r32 xPos = roundf(renderPosition.x * (float)ww);
			r32 yPos = roundf(-renderPosition.y * (float)wh);
			hm::vec2 start_pos = hm::vec2((xPos / 2.0f) + 800, (yPos / 2.0f) + 450 - get_font_size(font_id));
			
			if (m_textLength > 0) {
				
				render_text(font_id, m_text, m_textLength - 1, start_pos, m_textColor);
				font_rendering_flush();
			}
		}
	}
}
