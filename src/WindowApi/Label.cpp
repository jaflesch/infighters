#include "Label.h"
#include "WindowDiv.h"
#include "../font_render/os.h"

extern Window_Info window_info;

namespace linked
{
	/* LABELS */
	std::vector<FontRenderer*> Label::fontRenderers;

	Label::Label(
		const WindowDiv& div, 
		unsigned char* text, 
		int textLength, 
		hm::vec2& position,
		hm::vec4& textColor, 
		float fontSize, 
		float margin, 
		float padding) :
		m_text(text), 
		m_textLength(textLength),
		m_margin(margin),
		m_padding(padding), 
		m_div(div),
		m_position(position), 
		defaultLineSpace(true), 
		lineSpace(0), 
		m_textColor(textColor),
		m_yAdvance(0)
	{
		// Create font of size if non existant, use the existing if there is one
		m_fontRenderer = nullptr;
		for (unsigned int i = 0; i < Label::fontRenderers.size(); i++)
		{
			if (Label::fontRenderers[i]->getFontSize() == fontSize)
				m_fontRenderer = Label::fontRenderers[i];
		}
		if (m_fontRenderer == nullptr)
		{
			Label::fontRenderers.push_back(new FontRenderer(fontSize, FONT_QUALITY));
			m_fontRenderer = fontRenderers[fontRenderers.size() - 1];
		}
		// Set default line space to max ascender of the font
		setLineSpace(m_fontRenderer->getAscender());
	}

	Label::Label(const WindowDiv& div, unsigned char* text, int textLength, hm::vec2& position, float fontSize) :
		Label(div, text, textLength, position, DEFAULT_TEXTCOLOR, fontSize, 0, 0){}

	Label::Label(const WindowDiv& div, unsigned char* text, int textLength, float fontSize) :
		Label(div, text, textLength, hm::vec2(0, 0), DEFAULT_TEXTCOLOR, fontSize, 0, 0){}

	Label::Label(const WindowDiv& div) :
		Label(div, nullptr, 0, hm::vec2(0, 0), DEFAULT_TEXTCOLOR, DEFAULT_FONTSIZE, 0, 0){}

	void Label::cleanUp()
	{
		for (FontRenderer* fr : fontRenderers)
			delete fr;
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
			TextInfo ti = m_fontRenderer->RenderText(std::string((const char*)m_text),
				(int)(renderPosition.x * (float)ww), (int)(-renderPosition.y * (float)wh),
				rightLimit, m_textColor, Window::m_textShader, true);
			m_yAdvance = ti.num_rows;
		}
	}
}