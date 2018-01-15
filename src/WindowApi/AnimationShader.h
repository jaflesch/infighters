#pragma once
#include "Shader.h"
#include <hmath.h>
#include <string>
#include "Animation.h"

class AnimationShader : public Shader
{
public:
	AnimationShader(std::string fileName);
	~AnimationShader();
	virtual void update(hm::vec2& translation, Animation* animation);
	void bindTextures(GLuint textureId);
	void unbindTextures();
private:
	virtual void getUniformLocations();

	GLuint uniformTexture;
	GLuint uniformProjection, uniformModel;
	GLuint uniformTextureNumRows, uniformTextureOffset;

	hm::mat4 m_projection;
	hm::mat4 m_model;

public:
	inline hm::mat4& getProjection() { return this->m_projection; }
	inline hm::mat4& getModel() { return this->m_model; }
};

