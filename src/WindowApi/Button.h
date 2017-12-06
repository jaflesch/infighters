#pragma once
#include "FontRenderer.h"
#include "Label.h"
#include "Mesh.h"
#include "Primitive.h"
#include "../ResourceLoad/Texture.h"

#include <hmath.h>

namespace linked
{
class WindowDiv;
class Label;
class Button;
class Window;

struct Button_Info {
	int id;
	void* data;
	linked::Button* this_button;
};

class Button
{
public:
	Button(WindowDiv& div, Label* label, hm::vec2 position, int width, int height, hm::vec4 backgroundColor, int id);
	Button(WindowDiv& div, int width, int height);
	~Button();
	void render();
	void update();
	bool isHovered() const;

	static void flush();
	static void mouseCallback(int button, int action, int mods);
	Button_Info button_info;
private:
	WindowDiv& m_div;
	Window* m_window_base;
	int m_width, m_height;
	bool m_active;
	bool m_active_callback;
	bool m_render;
	bool m_is_toggle;
	bool m_toggled;
	float opacity;

	hm::vec2 m_position;
	Label* m_label;
	Mesh* m_buttonMesh;

	hm::vec4 m_labelTextColor,
		m_labelHeldTextColor,
		m_labelHoveredTextColor,
		m_labelNormalTextColor;

	hm::vec4 m_backgroundColor,
		m_backgroundHeldColor,
		m_backgroundHoveredColor,
		m_backgroundNormalColor;

	hm::vec4 m_backgroundInactiveColor,
		m_backgroundInactiveHeldColor,
		m_backgroundInactiveHoveredColor,
		m_backgroundInactiveNormalColor;

	hm::vec4 m_backgroundToggledColor,
		m_backgroundToggledHeldColor,
		m_backgroundToggledHoveredColor,
		m_backgroundToggledNormalColor;

	Texture* m_backgroundTexture, 
		*m_backgroundHeldTexture,
		*m_backgroundHoveredTexture,
		*m_backgroundNormalTexture;

	static bool clicked;
	static int mouseStatus;
	
	void(*clickedCallback)(void* arg);
public:
	Window* getWindow() { return m_window_base; }
	Label* getLabel() { return m_label; }
	Mesh& getButtonMesh()const{ return *(m_buttonMesh); }
	int getWidth()const{ return this->m_width; }
	int getHeight()const{ return this->m_height; }
	const hm::vec4& getBackgroundColor() const{ return this->m_backgroundColor; }
	const Texture* getBackgroundTexture()const{ return this->m_backgroundTexture; }
	hm::vec2 getScreenPosition() const;
	static int getMouseStatus() { return mouseStatus; }
	static bool getClicked() { return clicked; }
	void setClickedCallback(void(*callback)(void*));
	inline const hm::vec2 getPosition() const { return this->m_position; }
	inline void setOpacity(float opacity) { this->opacity = opacity; }
	inline float getOpacity() const { return this->opacity; }
	inline void setIsToggle(bool value) { this->m_is_toggle = value; }
	inline void toggle() { this->m_toggled = !this->m_toggled; }
	inline bool getIsToggled() { return this->m_toggled; }

	// setters for bg
	inline void setNormalBGColor(hm::vec4 color){ this->m_backgroundNormalColor = color; }
	inline void setHeldBGColor(hm::vec4 color){ this->m_backgroundHeldColor = color; }
	inline void setHoveredBGColor(hm::vec4 color){ this->m_backgroundHoveredColor = color; }

	inline void setInactiveNormalBGColor(hm::vec4 color) { this->m_backgroundInactiveNormalColor = color; }
	inline void setInactiveHeldBGColor(hm::vec4 color) { this->m_backgroundInactiveHeldColor = color; }
	inline void setInactiveHoveredBGColor(hm::vec4 color) { this->m_backgroundInactiveHoveredColor = color; }
	inline void setInactiveAllBGColor(hm::vec4 color) {
		this->m_backgroundInactiveNormalColor = color;
		this->m_backgroundInactiveHeldColor = color;
		this->m_backgroundInactiveHoveredColor = color; 
	}

	inline void setToggledNormalBGColor(hm::vec4 color) { this->m_backgroundToggledNormalColor = color; }
	inline void setToggledHeldBGColor(hm::vec4 color) { this->m_backgroundToggledHeldColor = color; }
	inline void setToggledHoveredBGColor(hm::vec4 color) { this->m_backgroundToggledHoveredColor = color; }
	inline void setToggledAllBGColor(hm::vec4 color) {
		this->m_backgroundToggledNormalColor = color;
		this->m_backgroundToggledHeldColor = color;
		this->m_backgroundToggledHoveredColor = color;
	}

	inline void setNormalBGTexture(Texture* texture){ this->m_backgroundNormalTexture = texture; }
	inline void setHeldBGTexture(Texture* texture){ this->m_backgroundHeldTexture = texture; }
	inline void setHoveredBGTexture(Texture* texture){ this->m_backgroundHoveredTexture = texture; }
	inline void setAllBGTexture(Texture* texture) {
		this->m_backgroundNormalTexture = texture;
		this->m_backgroundHeldTexture = texture;
		this->m_backgroundHoveredTexture = texture;
	}

	inline void setNormalTextColor(hm::vec4 color){ this->m_labelNormalTextColor = color; }
	inline void setHeldText(hm::vec4 color){ this->m_labelHeldTextColor = color; }
	inline void setHoveredTextColor(hm::vec4 color){ this->m_labelHoveredTextColor = color; }

	// getters for bg
	inline const hm::vec4& getNormalBGColor() const{ return this->m_backgroundNormalColor; }
	inline const hm::vec4& getHeldBGColor() const{ return this->m_backgroundHeldColor; }
	inline const hm::vec4& getHoveredBGColor() const{ return this->m_backgroundHoveredColor; }

	inline const hm::vec4& getInactiveNormalBGColor() const { return this->m_backgroundInactiveNormalColor; }
	inline const hm::vec4& getInactiveHeldBGColor() const { return this->m_backgroundInactiveHeldColor; }
	inline const hm::vec4& getInactiveHoveredBGColor() const { return this->m_backgroundInactiveHoveredColor; }

	inline const hm::vec4& getToggledNormalBGColor() const { return this->m_backgroundToggledNormalColor; }
	inline const hm::vec4& getToggledHeldBGColor() const { return this->m_backgroundToggledHeldColor; }
	inline const hm::vec4& getToggledHoveredBGColor() const { return this->m_backgroundToggledHoveredColor; }

	inline const Texture* getNormalBGTexture() const{ return this->m_backgroundNormalTexture; }
	inline const Texture* getHeldBGTexture() const{ return this->m_backgroundHeldTexture; }
	inline const Texture* getHoveredBGTexture() const{ return this->m_backgroundHoveredTexture; }

	inline const hm::vec4& getNormalTextColor() const{ return this->m_labelNormalTextColor; }
	inline const hm::vec4& getHeldText() const{ return this->m_labelHeldTextColor; }
	inline const hm::vec4& getHoveredTextColor() const{ return this->m_labelHoveredTextColor; }

	inline void setActive(bool active_change_color, bool active_callback = true) { this->m_active = active_change_color; this->m_active_callback = active_callback; }
	inline bool getActive() { return this->m_active; }
};

}

