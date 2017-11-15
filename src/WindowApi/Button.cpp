#include "Button.h"
#include "WindowDiv.h"
#include "..\input.h"
#include "../font_render/os.h"

extern Window_Info window_info;
extern Mouse_State mouse_state;

namespace linked
{
	int Button::mouseStatus;
	bool Button::clicked;

	Button::Button(const WindowDiv& div, Label* label, hm::vec2& position, int width, int height, hm::vec4& backgroundColor, int id)
		:
		m_div(div),
		m_label(label),
		m_position(position),
		m_width(width), m_height(height),
		clickedCallback(nullptr)
	{
		button_info.id = id;
		if(label)
			label->setPosition(m_position + label->getPosition());
		m_buttonMesh = new Mesh(new Quad(hm::vec3(0, 0, 0), (float)m_width, (float)m_height), true);
		
		m_backgroundTexture = nullptr;
		m_backgroundNormalTexture = m_backgroundTexture;
		m_backgroundHoveredTexture = m_backgroundTexture;
		m_backgroundHeldTexture = m_backgroundTexture;

		m_backgroundColor = backgroundColor;
		m_backgroundNormalColor = backgroundColor;
		m_backgroundHoveredColor = backgroundColor;
		m_backgroundHeldColor = backgroundColor;

		if(label)
			m_labelTextColor = label->getTextColor();
		m_labelNormalTextColor = m_labelTextColor;
		m_labelHoveredTextColor = m_labelTextColor;
		m_labelHeldTextColor = m_labelTextColor;
	}

	Button::Button(const WindowDiv& div, int width, int height)
		: Button(div, nullptr, hm::vec2(0, 0), width, height, hm::vec4(0, 0, 0, 1), 0){}

	Button::~Button()
	{
		if (m_buttonMesh)
			delete m_buttonMesh;
	}

	void Button::render()
	{
		if (m_label)
			m_label->render();
		isHovered();
	}

	void Button::update()
	{
		if (!this->m_div.getWindow().isFocusable()) {
			for (int i = 0; i < Window::openedWindows.size(); ++i) {
				if (Window::openedWindows[i]->getActive() && Window::openedWindows[i]->isHovered()) {
					if (Window::openedWindows[i] == &this->m_div.getWindow())
						continue;
					return;
				}
			}
		}

		if (Button::clicked)
		{
			//if (isHovered() && m_div.getWindow().isFocused())
			if(isHovered())
			{
				if (clickedCallback)
					clickedCallback(&button_info);
				Button::clicked = false;
			}
		}
		// Hovered
		//if (isHovered() && m_div.getWindow().isFocused() && mouseStatus == 0)
		if(isHovered() && mouseStatus == 0)
		{
			m_backgroundColor = m_backgroundHoveredColor;
			m_backgroundTexture = m_backgroundHoveredTexture;
			if (m_label)
				m_label->setTextColor(m_labelHoveredTextColor);
		}
		// Held
		//else if (isHovered() && m_div.getWindow().isFocused() && mouseStatus == 1)
		else if(isHovered() && mouseStatus == 1)
		{
			m_backgroundTexture = m_backgroundHeldTexture;
			m_backgroundColor = m_backgroundHeldColor;
			if (m_label)
				m_label->setTextColor(m_labelHeldTextColor);
		}
		// Not hovered
		else
		{
			m_backgroundTexture = m_backgroundNormalTexture;
			m_backgroundColor = m_backgroundNormalColor;
			if(m_label)
				m_label->setTextColor(m_labelNormalTextColor);
		}
	}

	void Button::flush()
	{
		Button::clicked = false;
	}

	bool Button::isHovered() const
	{
		hm::vec2 cursorPosition = hm::vec2(mouse_state.x, mouse_state.y);

		hm::vec2 buttonPosition = getScreenPosition();
		
		if (cursorPosition.x >= buttonPosition.x && cursorPosition.x <= buttonPosition.x + m_width &&
			cursorPosition.y >= buttonPosition.y && cursorPosition.y <= buttonPosition.y + m_height)
		{
			return true;
		}
#if 1	// Font debug
		glUseProgram(0);

		glLineWidth(1.0);

		glBegin(GL_LINES);

		glColor3f(1, 0.5f, 0);

		glVertex3f(cursorPosition.x, cursorPosition.y, 0.0f);	// Vertical left debug line
		glVertex3f(cursorPosition.x + 0.1f, cursorPosition.y, 0.0f);

		glEnd();

#endif
		return false;
	}

	hm::vec2 Button::getScreenPosition() const
	{
		hm::vec2 divPosition = m_div.getScreenPosition();
		hm::vec2 buttonPosition = divPosition + m_position;
		return buttonPosition;
	}

	void Button::mouseCallback(int button, int action, int mods)
	{
		if (button == 0 && action == 1)
			mouseStatus = 1;
		if (mouseStatus == 1 && button == 0 && action == 0)
		{
			mouseStatus = 0;
			clicked = true;
		}
	}

	void Button::setClickedCallback(void(*callback)(void*))
	{
		this->clickedCallback = callback;
	}
}