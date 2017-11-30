#pragma once
#include "Shader.h"
#include <string>
#include <hmath.h>
#include <ho_gl.h>

class FontShader : public Shader
{
public:
	FontShader(std::string);
	~FontShader();

	virtual void update();
	hm::vec2 clipTL;
	hm::vec2 clipBR;

private:
	virtual void getUniformLocations();

	GLuint uniformProjection;
	GLuint uniformTextTexture;
	GLuint uniformTextColor;
	GLuint uniformModel;

	GLuint uniformClipTopLeft;
	GLuint uniformClipBotRight;

	hm::vec4 m_textColor;
public:
	/* getters and setters */
	inline hm::vec4& getColor() { return this->m_textColor; }
	inline void setTextColor(const hm::vec4& color) { this->m_textColor = color; }
};

