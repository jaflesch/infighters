#include "WindowDiv.h"
#include "../font_render/os.h"

extern Window_Info window_info;

namespace linked
{
	WindowDiv::WindowDiv(
		Window& window, 
		int width, int height,
		float margin, float padding,
		hm::vec2 position,
		hm::vec4 backGroundColor, 
		unsigned int hints) :
		m_window(window), 
		m_margin(margin), 
		m_padding(padding),
		m_width(width), 
		m_height(height), 
		m_position(position), 
		m_backGroundColor(backGroundColor), 
		m_render(true),
		m_backgroundTexture(nullptr)
	{
		handleDivHints(hints);
		divMesh = new Mesh(new Quad(hm::vec3(0, 0, 0), (float)width, (float)height), true);
		m_opacity = 1.0f;

		borderMesh = 0;// new Mesh(new Border(hm::vec3(0, 0, 0), width * 2, height * 2, 2.0f, 2.0f, 2.0f, 2.0f));
	}

	void WindowDiv::render()
	{
		if (!m_render) return;
		WindowShader* ws = m_window.getWindowShader();

		ws->useShader();
		ws->activateAlphaBlend();

		if (m_backgroundTexture != nullptr)
			ws->bindTextures(this->m_backgroundTexture->textureID);
		if (m_render)
		{
			// Clips to edge of window and sets the correct blending mode to render divs
			ws->clipTL = m_window.getWindowBasePosition(0, 0);
			ws->clipBR = m_window.getWindowBasePosition((float)m_window.getWidth(), (float)m_window.getHeight());
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			// Prepare the div and render it
			ws->setTextColor(m_backGroundColor);
			if (m_backgroundTexture != nullptr)
				ws->bindTextures(m_backgroundTexture->textureID);
			
			hm::vec2 rp = getRelativePosition();
			float ww_ = roundf(window_info.width / 2.0f);
			float wh_ = roundf(window_info.height / 2.0f);
			rp.x /= ww_;
			rp.y /= wh_;
			
			ws->setOpacity(m_opacity);
			hm::vec2 relative_pos = getRelativePosition();
			ws->update(relative_pos);
			divMesh->render();
			ws->setOpacity(1.0f);

			if (m_backgroundTexture != nullptr)
				ws->unbindTextures();

		}

		for (Button* b : buttons)
		{
			hm::vec2 v = getRelativePosition();
			v.x -= ((this->m_width - b->getWidth()) / 2.0f) - b->getPosition().x;
			v.y -= ((this->m_height - b->getHeight()) / 2.0f) - b->getPosition().y;
			ws->setTextColor(b->getBackgroundColor());
			if (b->getBackgroundTexture() != nullptr)
				ws->bindTextures(b->getBackgroundTexture()->textureID);
		
			ws->setOpacity(b->getOpacity());
			ws->update(v);
			b->getButtonMesh().render();
			ws->setOpacity(1.0f);

			if (b->getBackgroundTexture() != nullptr)
				ws->unbindTextures();
		}

		if (m_render) {
			hm::vec2 relative_pos = getRelativePosition();
			if (borderMesh) {
				ws->setTextColor(borderMesh->getBorder()->getBGColor());
				ws->update(relative_pos);
				ws->useTexture = 0;
				borderMesh->render();
			}
		}

		if (m_backgroundTexture != nullptr)
			ws->unbindTextures();
		ws->deactivateAlphaBlend();
		ws->stopShader();

		// Clip planes where the window ends
		Window::m_textShader->useShader();
		hm::vec2 w_clipTL = m_window.getWindowBasePosition(0, 0);
		hm::vec2 w_clipBR = m_window.getWindowBasePosition((float)m_window.getWidth(), (float)m_window.getHeight());
		
		Window::m_textShader->clipTL = w_clipTL;
		Window::m_textShader->clipBR = w_clipBR;
		
		Window::m_textShader->update();

		// Render all button labels
		for (Button* b : buttons)
			b->render();

		// Render all labels in order of creation
		for (Label* l : labels)
			l->render();

		ws->clipTL = hm::vec2(-1, -1);
		ws->clipBR = hm::vec2((float)window_info.width, (float)window_info.height);

		Window::m_textShader->stopShader();
		
	}

	void WindowDiv::update()
	{
		if (!m_render) return;
		setAbsolutePosition(m_position);
		for (Button* b : buttons)
			b->update();
	}

	void WindowDiv::setAbsolutePosition(hm::vec2 absolutePos)
	{
		if (m_anchor_left || m_anchor_right || m_anchor_top || m_anchor_bottom || m_center_x || m_center_y)
		{
			hm::vec2 result = hm::vec2(m_position.x, m_position.y);

			// Handle anchoring
			if (m_anchor_left)
				result.x -= (m_window.getWidth() - m_width - m_margin) / 2.0f - m_window.getPosition().x;
			else if (m_anchor_right)
				result.x -= (m_width - m_window.getWidth() + m_margin) / 2.0f - m_window.getPosition().x;

			if (m_anchor_top)
				result.y -= (m_window.getHeight() - m_height - m_margin) / 2.0f - m_window.getPosition().y;
			else if (m_anchor_bottom)
				result.y -= (m_height - m_window.getHeight() + m_margin) / 2.0f - m_window.getPosition().y;
			if (m_center_x)
				result.x = m_window.getPosition().x;
			if (m_center_y)
				result.y = m_window.getPosition().y;
			m_windowRelativePosition = result;
		}
		else
		{
			m_windowRelativePosition = m_position;
		}

	}

	WindowDiv::~WindowDiv()
	{
		if (divMesh)
			delete divMesh;
		//if (m_backgroundTexture)
		//	delete m_backgroundTexture;

		for (Label* l : labels)
		{
			delete l;
			l = nullptr;
		}
		for (Button* b : buttons)
		{
			delete b;
			b = nullptr;
		}
	}

	void WindowDiv::handleDivHints(unsigned int hints)
	{
		m_anchor_left = (hints & DIV_ANCHOR_LEFT) ? true : false;
		m_anchor_right = (hints & DIV_ANCHOR_RIGHT) ? true : false;
		m_anchor_top = (hints & DIV_ANCHOR_TOP) ? true : false;
		m_anchor_bottom = (hints & DIV_ANCHOR_BOTTOM) ? true : false;
		m_float_left = (hints & DIV_FLOAT_LEFT) ? true : false;
		m_float_right = (hints & DIV_FLOAT_RIGHT) ? true : false;
		m_float_top = (hints & DIV_FLOAT_TOP) ? true : false;
		m_float_bottom = (hints & DIV_FLOAT_BOTTOM) ? true : false;
		m_center_y = (hints & DIV_CENTER_Y) ? true : false;
		m_center_x = (hints & DIV_CENTER_X) ? true : false;
	}

	hm::vec2 WindowDiv::getDivBasePosition(float xoffset, float yoffset) const
	{
		hm::vec2 basePos = m_windowRelativePosition;
		basePos.x += xoffset;
		basePos.y += yoffset;
		basePos.x -= m_width / 2.0f;
		basePos.y -= m_height / 2.0f;

		basePos.x /= window_info.width / 2.0f;
		basePos.y /= window_info.height / 2.0f;

		return basePos;
	}

	hm::vec2 WindowDiv::getScreenPosition() const
	{
		hm::vec2 divPosition = const_cast<hm::vec2&>(m_windowRelativePosition) + hm::vec2(
			window_info.width / 2.0f,
			window_info.height / 2.0f);
		divPosition.x -= m_width / 2.0f;
		divPosition.y -= m_height / 2.0f;
		return divPosition;
	}

	bool WindowDiv::isButtonHovered()
	{
		for (Button* b : buttons)
		{
			if (b->isHovered())
				return true;
		}
		return false;
	}

	void WindowDiv::createBorder(float l, float r, float t, float b) {
		borderMesh = new Mesh(new Border(hm::vec3(0, 0, 0), (float)m_width * 2, (float)m_height * 2, l * 2, r * 2, t * 2, b * 2));
	}
}