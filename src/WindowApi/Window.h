#pragma once
#include "Primitive.h"
#include "WindowShader.h"
#include "FontShader.h"
#include "FontRenderer.h"
#include "WindowDiv.h"
#include "Label.h"
#include "Mesh.h"
#include <ho_gl.h>

#include <vector>
#include <hmath.h>

class Shader;
class WindowShader;
class Mesh;
class FontRenderer;

namespace linked
{
class WindowDiv;
class Label;

enum windowHints{
	W_RESIZEABLE		= 0x01,
	W_MOVABLE			= 0x02,
	W_HEADER			= 0x04,
	W_CLOSE_BUTTON		= 0x08,
	W_SCROLLBAR			= 0x10,
	W_BORDER			= 0x20,
	W_UNFOCUSABLE		= 0x40,
};

enum divHints{
	DIV_ANCHOR_LEFT		= 0x01,
	DIV_ANCHOR_RIGHT	= 0x02,
	DIV_ANCHOR_TOP		= 0x04,
	DIV_ANCHOR_BOTTOM	= 0x08,
	DIV_FLOAT_LEFT		= 0x10,
	DIV_FLOAT_RIGHT		= 0x20,
	DIV_FLOAT_TOP		= 0x40,
	DIV_FLOAT_BOTTOM	= 0x80,
	DIV_CENTER_Y		= 0x100,
	DIV_CENTER_X		= 0x200
};

class Window
{
#define WINDOW_SHADER_PATH "./res/shaders/windowapi/window_shader"
#define FONT_SHADER_PATH "./res/shaders/windowapi/font_shader"
#define DEFAULT_TITLE_COLOR 0.2f, 0.2f, 0.2f, 1.0f
#define DEFAULT_BORDER_SIZE 2		// pixels
#define DEFAULT_BORDER_COLOR 0.0f, 0.0f, 0.0f, 1.0f
#define DEFAULT_HEADER_SIZE 20.0f	// pixels
public: 
	// Static
	// Methods
	static bool update_background;
	static void renderWindows();
	static void updateWindows();
	// Data
	static std::vector<Window*> openedWindows;
	static std::vector<Window*> backgroundWindows;
	static WindowShader* m_windowShader;
	static FontShader* m_textShader;
	static void linkedWindowDestroy();
	static void linkedWindowInit();
public: 
	// Methods
	Window(
		int width, 
		int height, 
		const hm::vec2& position,
		const hm::vec4& backgroundColor,
		unsigned char* title, 
		int titleLength,
		unsigned int hints);
	~Window();
	unsigned int createDiv(
		const hm::vec2& relativePos,
		divHints hints,
		float defaultPadding,
		float defaultMargin);
	void render();
	void update();
		
	void mouseCallback(int button, int action, int mods);
private: 
	// Data
	int m_width, m_height;
	int m_titleLength;
	bool m_titleCentered;
	unsigned char* m_title;
	hm::vec4 m_titleColor;
	hm::vec2 m_position;
	hm::vec2 m_screenPosition;
	hm::vec4 m_backgroundColor;
	hm::vec4 m_borderColor;
	float m_borderSizeX, m_borderSizeY;
	float m_headerSize;
	bool m_attached;					// Mouse related
	hm::vec2 m_cursorPosWhenAttached;	// -
	hm::vec2 m_posWhenAttached;		// -

	bool h_resizeable, h_movable, h_header, h_closeButton,	// hints
		h_scrollbar, h_border, h_focusable;								// -
	bool focused;
	bool m_active;
	Mesh* m_windowMesh, *m_borderMesh;
	FontRenderer* m_fontRenderer;
public:
	std::vector<WindowDiv*> divs;
	bool isFocused();
	bool isHovered();
private: 
	// Methods
	void attachMouse();
	void detachMouse();
	
	void handleWindowHints(unsigned int hints);		// hints related
	void setupBorder();

public: // Getters and Setters
	void setFocus();
	inline bool isFocusable() const { return h_focusable; }
	inline bool getActive() const { return this->m_active; }
	inline void setActive(bool value) { this->m_active = value; }
	inline hm::vec2 getPosition() const { return this->m_position; }
	inline hm::vec2 getScreenPosition() const { return this->m_screenPosition; }
	void setPosition(const hm::vec2& newPosition);

	inline int getWidth()const{ return this->m_width; }
	inline int getHeight()const{ return this->m_height; }
	inline void setWidth(int value) { this->m_width = value; m_windowMesh->getQuad()->updateQuad(value, m_height); m_windowMesh->updateQuad(); }
	inline void setHeight(int value) { this->m_height = value; m_windowMesh->getQuad()->updateQuad(m_width, value); m_windowMesh->updateQuad(); }
	inline void setWidthHeight(int width, int height) { 
		this->m_width = width; 
		this->m_height = height; 
		m_windowMesh->getQuad()->updateQuad(width, height); 
		m_windowMesh->updateQuad(); 
	}

	inline hm::vec4 getBackGroundColor() const { return this->m_backgroundColor; }
	inline void setBackGroundColor(const hm::vec4& newBackGroundColor) { this->m_backgroundColor = newBackGroundColor; }
	
	inline float getBorderSizeX() const{ return this->m_borderSizeX; }
	void setBorderSizeX(float newBorderSize);
	inline float getBorderSizeY() const{ return this->m_borderSizeY; }
	void setBorderSizeY(float newBorderSize);
	
	inline void setBorderColor(hm::vec4& newBorderColor){ this->m_borderColor = newBorderColor; }
	inline hm::vec4 getBorderColor()const{ return this->m_borderColor; }
	
	void setTitle(unsigned char* title, int titleLength);
	inline void setTitleColor(const hm::vec4& titleColor){ m_titleColor = titleColor; }
	
	inline bool isFocused() const{ return focused; }
	inline bool isAttached() const { return m_attached; }

	inline void setTitleCentered(bool value){ this->m_titleCentered = value; }

	hm::vec2 getWindowBasePosition(float xoffset, float yoffset) const;
	WindowShader* getWindowShader()const{ return m_windowShader; }
};
}

