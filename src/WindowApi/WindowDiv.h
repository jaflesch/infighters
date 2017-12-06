#pragma once
#include "Button.h"
#include "Label.h"
#include "Mesh.h"
#include "../ResourceLoad/Texture.h"
#include "Window.h"
#include "WindowShader.h"

#include <vector>
#include <hmath.h>

class Mesh;

namespace linked
{
class Window;
class Label;
class Button;

class WindowDiv
{
public:
	// Methods
	WindowDiv(Window& window, int width, int height, float margin, float padding,
		hm::vec2 position, hm::vec4 backGroundColor, unsigned int hints);
	~WindowDiv();
	void render();
	void update();
	bool isButtonHovered();

	// Getters and setters
	hm::vec2 getRelativePosition() const { return m_windowRelativePosition; }
	hm::vec2 getPosition() const { return this->m_position; }
	inline void setWindowRelativePosition(hm::vec2 relativePos) { this->m_windowRelativePosition = relativePos; }
	void setAbsolutePosition(hm::vec2 absolutePos);
	float getAlphaValue() const { return this->m_backGroundColor.a; }
	hm::vec2 getDivBasePosition(float xoffset, float yoffset) const;
	inline int getWidth() const{ return this->m_width; }
	inline int getHeight() const { return this->m_height; }
	inline void setWidth(int value) { this->m_width = value; divMesh->getQuad()->updateQuad(value, m_height); divMesh->updateQuad(); }
	inline void setHeight(int value) { this->m_height = value; divMesh->getQuad()->updateQuad(m_width, value); divMesh->updateQuad(); }
	inline void setWidthHeight(int width, int height) { this->m_width = width; this->m_height = height; divMesh->getQuad()->updateQuad(width, height); divMesh->updateQuad(); }
	bool m_render;
	const Window& getWindow()const{ return m_window; }
	Window* getWindowPtr() { return &m_window; }
	hm::vec2 getScreenPosition() const;
	std::vector<Label*>& getLabels(){ return this->labels; }
	std::vector<Button*>& getButtons(){ return this->buttons; }
	void setBackgroundColor(hm::vec4 color){ this->m_backGroundColor = color; }
	hm::vec4 getBackgroundColor()const { return this->m_backGroundColor; }
	void setBackgroundTexture(Texture* texture){ this->m_backgroundTexture = texture; }
	Texture* getBackgroundTexture(){ return this->m_backgroundTexture; }
	inline void setOpacity(float opacity) { this->m_opacity = opacity; }
	inline float getOpacity() const { return this->m_opacity; }
private:
	// Data
	float m_margin, m_padding;
	int m_width, m_height;
	float m_opacity;
	hm::vec2 m_windowRelativePosition;
	hm::vec2 m_position;		// Position in pixel coords
	hm::vec4 m_backGroundColor;
	Window& m_window;

	Mesh* divMesh;
	Texture* m_backgroundTexture;

	std::vector<Label*> labels;
	std::vector<Button*> buttons;
	// hints
	bool m_anchor_left, m_anchor_right, m_anchor_top,
		m_anchor_bottom, m_float_left, m_float_right,
		m_float_top, m_float_bottom, m_center_y, m_center_x;
	
private:
	// Methods
	void handleDivHints(unsigned int hints);
};
}
