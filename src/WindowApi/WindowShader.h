#pragma once
#include "Shader.h"

#include <hmath.h>

#include <string>

class WindowShader : public Shader
{
public:
	WindowShader(std::string fileName);
	~WindowShader();
	virtual void update(hm::vec2& translation);
	void bindTextures(GLuint textureId);
	void unbindTextures();

	hm::vec2 clipTL;
	hm::vec2 clipBR;	
private:
	virtual void getUniformLocations();
	GLuint uniformTexture, uniformUseTexture;

	GLuint uniformProjection, uniformTextColor, uniformModel;
	GLuint uniformClipTopLeft, uniformClipBotRight;
	GLuint uniformOpacity;

	hm::vec4 m_backgroundColor;
	hm::mat4 m_projection;
	hm::mat4 m_model;
	float m_opacity;

	int useTexture;
public:
	inline void setOpacity(float opacity) { this->m_opacity = opacity; }
	inline hm::mat4& getProjection(){ return this->m_projection; }
	inline hm::mat4& getModel() { return this->m_model; }
	inline hm::vec4& getBackgroundColor() { return this->m_backgroundColor; }
	inline void setTextColor(const hm::vec4& color){ this->m_backgroundColor = color; }
};

