#pragma once
#include "FontRenderer.h"
#include "Label.h"
#include "Mesh.h"
#include "Primitive.h"
#include "..\ResourceLoad\Texture.h"

#include <hmath.h>

namespace linked
{
class WindowDiv;
class Label;

struct Button_Info {
	int id;
	void* data;
};

class Button
{
public:
	Button(const WindowDiv& div, Label* label, hm::vec2& position, int width, int height, hm::vec4& backgroundColor, int id);
	Button(const WindowDiv& div, int width, int height);
	~Button();
	void render();
	void update();
	bool isHovered() const;

	static void flush();
	static void mouseCallback(int button, int action, int mods);
	Button_Info button_info;
private:
	const WindowDiv& m_div;
	int m_width, m_height;
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

	Texture* m_backgroundTexture, 
		*m_backgroundHeldTexture,
		*m_backgroundHoveredTexture,
		*m_backgroundNormalTexture;

	static bool clicked;
	static int mouseStatus;
	
	void(*clickedCallback)(void* arg);
public:
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

	// setters for bg
	inline void setNormalBGColor(hm::vec4& color){ this->m_backgroundNormalColor = color; }
	inline void setHeldBGColor(hm::vec4& color){ this->m_backgroundHeldColor = color; }
	inline void setHoveredBGColor(hm::vec4& color){ this->m_backgroundHoveredColor = color; }

	inline void setNormalBGTexture(Texture* texture){ this->m_backgroundNormalTexture = texture; }
	inline void setHeldBGTexture(Texture* texture){ this->m_backgroundHeldTexture = texture; }
	inline void setHoveredBGTexture(Texture* texture){ this->m_backgroundHoveredTexture = texture; }

	inline void setNormalTextColor(hm::vec4& color){ this->m_labelNormalTextColor = color; }
	inline void setHeldText(hm::vec4& color){ this->m_labelHeldTextColor = color; }
	inline void setHoveredTextColor(hm::vec4& color){ this->m_labelHoveredTextColor = color; }

	// getters for bg
	inline const hm::vec4& getNormalBGColor() const{ return this->m_backgroundNormalColor; }
	inline const hm::vec4& getHeldBGColor() const{ return this->m_backgroundHeldColor; }
	inline const hm::vec4& getHoveredBGColor() const{ return this->m_backgroundHoveredColor; }

	inline const Texture* getNormalBGTexture() const{ return this->m_backgroundNormalTexture; }
	inline const Texture* getHeldBGTexture() const{ return this->m_backgroundHeldTexture; }
	inline const Texture* getHoveredBGTexture() const{ return this->m_backgroundHoveredTexture; }

	inline const hm::vec4& getNormalTextColor() const{ return this->m_labelNormalTextColor; }
	inline const hm::vec4& getHeldText() const{ return this->m_labelHeldTextColor; }
	inline const hm::vec4& getHoveredTextColor() const{ return this->m_labelHoveredTextColor; }
};

}

