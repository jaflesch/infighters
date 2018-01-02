#include "Window.h"
#include "../common.h"
#include "../input.h"
#include "../font_render/os.h"

extern Window_Info window_info;
extern Mouse_State mouse_state;

using namespace linked;
std::vector<linked::Window*> linked::Window::openedWindows;
std::vector<linked::Window*> linked::Window::backgroundWindows;
FontShader* linked::Window::m_textShader = nullptr;
WindowShader* linked::Window::m_windowShader = nullptr;
bool linked::Window::update_background = true;
int linked::Window::id_gen = 0;

namespace linked {

Window::Window(
	int width, int height,
	const hm::vec2& position,
	const hm::vec4& backgroundColor,
	unsigned char* title,
	int titleLength,
	unsigned int hints)
	:
	m_width(width), m_height(height),
	m_backgroundColor(backgroundColor),
	m_attached(false)
{
	id = Window::id_gen;
	Window::id_gen += 1;
	m_windowShader->setTextColor(this->m_backgroundColor);
	m_windowMesh = new Mesh(new Quad(hm::vec3(0, 0, 0), (float)width, (float)height), true);

	m_fontRenderer = new FontRenderer(35, 512);

	setTitle(title, titleLength);
	setTitleColor(hm::vec4(DEFAULT_TITLE_COLOR));

	// Set Initial Position
	m_screenPosition = position;
	m_position = hm::vec2(
		position.x + (width - window_info.width) / 2.0f,
		position.y + (height - window_info.height) / 2.0f);

	handleWindowHints(hints);

	m_active = true;

	if (h_focusable) {
		// When this window is created it will be focused automatically
		focused = true;
		// Unfocus all of the others
		for (unsigned int i = 0; i < openedWindows.size(); i++)
			openedWindows[i]->focused = false;
		Window::openedWindows.push_back(this);
	} else {
		Window::backgroundWindows.push_back(this);
	}

}

Window::~Window()
{
	delete this->m_windowMesh;
	delete this->m_fontRenderer;
	delete[] m_title;

	if (m_borderMesh != nullptr && h_border)
		delete m_borderMesh;

	// delete divs
	for (WindowDiv* d : divs)
		delete d;
}

void Window::linkedWindowInit()
{
	linked::Window::m_textShader = new FontShader(FONT_SHADER_PATH);
	m_windowShader = new WindowShader(WINDOW_SHADER_PATH);
}

void Window::linkedWindowDestroy()
{
	for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
		delete linked::Window::openedWindows[i];
	delete Window::m_windowShader;
	delete Window::m_textShader;
	Label::cleanUp();
}

void Window::render()
{
	if (!m_active)
		return;
	Window::m_windowShader->useShader();
	Window::m_windowShader->activateAlphaBlend();

	// render window on top
	Window::m_windowShader->setTextColor(m_backgroundColor);
	Window::m_windowShader->update(m_position);
	m_windowMesh->render();

	// Render Inverse Mask ------------------------------------------------
	glColorMask(false, false, false, true);									// sets a clear mask to only render on alpha channel
	hm::vec4 clearColor = hm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, m_borderColor.a);// clears the color buffer and sets the alpha to 1 for inverse mask, 0 for normal mask
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);			// needed to mask off colors

	Window::m_windowShader->setTextColor(hm::vec4(0, 0, 0, 0));					// alpha channel must be 1 for normal mask and 0 for inverted
	Window::m_windowShader->update(m_position);
	m_windowMesh->render();													// render the mask to alpha channel

	glColorMask(true, true, true, true);									// restores the normal rendering to color buffer

	// render border behind
	if (h_border)
	{
		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		Window::m_windowShader->setTextColor(m_borderColor);
		Window::m_windowShader->update(m_position);
		m_borderMesh->render();
	}
	Window::m_windowShader->deactivateAlphaBlend();
	Window::m_windowShader->stopShader();
	// End of inverse mask rendering --------------------------------------

	// render divs
	for (WindowDiv* d : divs)
	{
		d->update();
		d->render();
	}

	// Set clipping planes back to all the screen
	Window::m_windowShader->clipTL = hm::vec2(-1, -1);
	Window::m_windowShader->clipBR = hm::vec2((float)window_info.width, (float)window_info.height);

	// render window title in header if header enabled
	if (h_header)
	{
		// Set Clipping planes to all the context (to be always rendered)
		Window::m_textShader->useShader();
		Window::m_textShader->clipTL = hm::vec2(-1, -1);
		Window::m_textShader->clipBR = hm::vec2((float)window_info.width, (float)window_info.height);
		Window::m_textShader->update();

		float rightLimit = getWindowBasePosition((float)m_width, 0).x;
		float xpos = m_position.x * 2.0f - m_width;
		float ypos = getPosition().y * 2.0f - m_height - 10.0f;

		if (m_titleCentered)
			xpos += m_width;

		hm::vec2 text_info = m_fontRenderer->RenderTextGetInfo((const char*)m_title, m_titleLength);
		m_fontRenderer->RenderText((const char*)m_title, m_titleLength, xpos - text_info.x, -ypos, rightLimit, m_titleColor, m_textShader, false);
		Window::m_textShader->stopShader();
	}
}

void Window::update()
{
	if (!m_active)
		return;

	if (!h_focusable) {
		return;
	} else if(isHovered()) {
		Window::update_background = false;
	} else {
		Window::update_background = true;
	}

	// if the mouse is attached to the window, move it accordingly
	if (m_attached && h_movable && focused)
	{
		update_background = false;
		hm::vec2 cursorPosition = hm::vec2((float)mouse_state.x, (float)mouse_state.y);
		hm::vec2 delta = m_cursorPosWhenAttached - cursorPosition;
		setPosition(m_posWhenAttached - delta);
		// updates the screen position
		m_screenPosition = m_position + hm::vec2(window_info.width / 2.0f, window_info.height / 2.0f);
		m_screenPosition = m_screenPosition - hm::vec2(m_width / 2.0f, m_height / 2.0f);
	} else {
		update_background = true;
	}
}

unsigned int Window::createDiv(
	const hm::vec2& relativePos,
	divHints hints,
	float defaultPadding,
	float defaultMargin)
{
	// create div
	return 0;
}

void Window::handleWindowHints(unsigned int hints)
{
	h_resizeable = (hints & W_RESIZEABLE) ? true : false;
	h_movable = (hints & W_MOVABLE) ? true : false;
	h_header = (hints & W_HEADER) ? true : false;
	h_closeButton = (hints & W_CLOSE_BUTTON) ? true : false;
	h_scrollbar = (hints & W_SCROLLBAR) ? true : false;
	h_border = (hints & W_BORDER) ? true : false;
	h_focusable = (hints & W_UNFOCUSABLE) ? false : true;

	if (h_border)
		setupBorder();
}

void Window::mouseCallback(int button, int action, int mods)
{
	//if (button == GLFW_MOUSE_BUTTON_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	if (button == 0 && (action == 1 || action == 1))
	{
		// if any button is hovered window should not be moved but should be focused
		for (WindowDiv* d : divs)
			if (d->isButtonHovered())
			{
				attachMouse();
				detachMouse();
				return;
			}

		attachMouse();
		if (action == 1)
		//if (action == GLFW_PRESS)
		{
			m_cursorPosWhenAttached = hm::vec2((float)mouse_state.x, (float)mouse_state.y);
			m_posWhenAttached = m_position;
		}
	}
	else
		detachMouse();
}

void Window::setFocus()
{
	int thisIndex = 0;
	for (unsigned int i = 0; i < openedWindows.size(); i++) {
		openedWindows[i]->focused = false;
		if (openedWindows[i] == this)
			thisIndex = i;
	}

	openedWindows.erase(openedWindows.begin() + thisIndex);
	openedWindows.push_back(this);

	focused = true;
}

void Window::attachMouse()
{
	if (!h_focusable) {
		return;
	}

	// Verify if the window is focused and is beeing hovered
	if (isHovered() && m_active)
	{
		// If window is focused and is hovered, don't do anything, state is correct as it is
		for (unsigned int i = 0; i < openedWindows.size(); i++)
		{
			if (openedWindows[i]->isFocused() && openedWindows[i]->isHovered())
			{
				// If it is this windows, then attach it for moving
				if (openedWindows[i] == this)
					m_attached = true;
				return;
			}
		}
		// If this is the first hovered window then handle it, otherwise another window is in front
		// and also being hovered, in this case return
		int thisIndex = 0;
		for (size_t i = openedWindows.size() - 1; i >= 0; i--)
		{
			if (openedWindows[i] == this)
			{
				thisIndex = (int)i;
				break;
			}
			if (openedWindows[i]->isHovered() && openedWindows[i]->m_active)
				return;
		}

		// This window should be focused, unfocuse all others and put this at the end of the list
		// to be rendered first, also set this to focused
		for (unsigned int i = 0; i < openedWindows.size(); i++)
			openedWindows[i]->focused = false;

		openedWindows.erase(openedWindows.begin() + thisIndex);
		openedWindows.push_back(this);

		m_attached = true;
		focused = true;
	}

}
void Window::detachMouse()
{
	m_attached = false;
}
bool Window::isHovered()
{
	hm::vec2 cursorPosition = hm::vec2((float)mouse_state.x, (float)mouse_state.y);
	hm::vec2 cursorPosRefWindow = hm::vec2(
		cursorPosition.x + (m_width - window_info.width) / 2.0f,
		cursorPosition.y + (m_height - window_info.height) / 2.0f);

	if (cursorPosRefWindow.x >= m_position.x && cursorPosRefWindow.x <= m_position.x + m_width &&
		cursorPosRefWindow.y >= m_position.y && cursorPosRefWindow.y <= m_position.y + m_height)
	{
		return true;
	}
	else if (h_border)
	{
		// Makes sure it tests for header
		float headerSize = (h_header) ? m_headerSize : 0;
		if (cursorPosRefWindow.x >= m_position.x - m_borderSizeX && cursorPosRefWindow.x <= m_position.x + m_width + m_borderSizeX &&
			cursorPosRefWindow.y >= m_position.y - m_borderSizeY - headerSize && cursorPosRefWindow.y <= m_position.y + m_height + m_borderSizeY)
		{
			return true;
		}
	}
	return false;
}
bool Window::isFocused()
{
	return focused;
}
void Window::setPosition(const hm::vec2 newPosition)
{
	m_position = newPosition;
}
void Window::setTitle(unsigned char* title, int titleLength)
{
	m_title = new unsigned char[titleLength];
	m_titleLength = titleLength;
	for (int i = 0; i < titleLength; i++)
	{
		m_title[i] = title[i];
	}
}
hm::vec2 Window::getWindowBasePosition(float xoffset, float yoffset) const
{
	hm::vec2 basePos = m_position;
	basePos.x += xoffset;
	basePos.y += yoffset;
	basePos.x -= m_width / 2.0f;
	basePos.y -= m_height / 2.0f;

	basePos.x /= window_info.width / 2.0f;
	basePos.y /= window_info.height / 2.0f;

	return basePos;
}

/* Hints handling */
void Window::setupBorder()
{
	float borderSizeX = DEFAULT_BORDER_SIZE * 2.0f;
	float borderSizeY = DEFAULT_BORDER_SIZE * 2.0f;

	m_borderSizeX = DEFAULT_BORDER_SIZE;
	m_borderSizeY = DEFAULT_BORDER_SIZE;

	Quad* borderQuad = new Quad(hm::vec3(0, 0, 0), m_width + borderSizeX, m_height + borderSizeY);

	if (h_header)
	{
		IndexedModel* im = borderQuad->getIndexedModel();
		im->positions[0].y = im->positions[0].y + DEFAULT_HEADER_SIZE * 2.0f;	// topLeft corner
		im->positions[1].y = im->positions[1].y + DEFAULT_HEADER_SIZE * 2.0f;	// topRight corner
		m_headerSize = DEFAULT_HEADER_SIZE;
	}

	m_borderMesh = new Mesh(borderQuad, true);
	m_borderColor = hm::vec4(DEFAULT_BORDER_COLOR);
}

void Window::renderWindows()
{
	if (backgroundWindows.size() > 0)
		for (unsigned int i = 0; i < backgroundWindows.size(); ++i)
			backgroundWindows[i]->render();

	if (openedWindows.size() > 0)
		for (unsigned int i = 0; i < openedWindows.size(); i++)
			openedWindows[i]->render();


	Button::flush();
}

void Window::updateWindows()
{
	for (Window* w : Window::openedWindows)
		w->update();

	if (update_background) {
		for (Window* w : Window::backgroundWindows)
			w->update();
	}
}

void Window::setBorderSizeY(float newBorderSize)
{
	IndexedModel* im = m_borderMesh->getQuad()->getIndexedModel();

	float currentSizeY = m_borderSizeY * 2.0f;

	im->positions[0].y = im->positions[0].y - currentSizeY + (newBorderSize  * 2.0f);
	im->positions[1].y = im->positions[1].y - currentSizeY + (newBorderSize  * 2.0f);
	im->positions[2].y = im->positions[2].y + currentSizeY - (newBorderSize  * 2.0f);
	im->positions[3].y = im->positions[3].y + currentSizeY - (newBorderSize  * 2.0f);

	glBindBuffer(GL_ARRAY_BUFFER, m_borderMesh->getVertexBufferID());
	glBufferSubData(GL_ARRAY_BUFFER, 0, im->positions.size() * sizeof(float) * 3, &im->positions[0]);

	this->m_borderSizeY = newBorderSize;
}
void Window::setBorderSizeX(float newBorderSize)
{
	IndexedModel* im = m_borderMesh->getQuad()->getIndexedModel();

	float currentSizeX = m_borderSizeX * 2.0f;

	im->positions[0].x = im->positions[0].x + currentSizeX - (newBorderSize  * 2.0f);
	im->positions[1].x = im->positions[1].x - currentSizeX + (newBorderSize  * 2.0f);
	im->positions[2].x = im->positions[2].x + currentSizeX - (newBorderSize  * 2.0f);
	im->positions[3].x = im->positions[3].x - currentSizeX + (newBorderSize  * 2.0f);

	glBindBuffer(GL_ARRAY_BUFFER, m_borderMesh->getVertexBufferID());
	glBufferSubData(GL_ARRAY_BUFFER, 0, im->positions.size() * sizeof(float) * 3, &im->positions[0]);

	this->m_borderSizeX = newBorderSize;
}

} // namespace linked
