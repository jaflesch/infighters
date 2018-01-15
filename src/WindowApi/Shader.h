#pragma once
#include <ho_gl.h>
#include <string>
class Entity;

class Shader
{
public:
	Shader(std::string);
	~Shader();
	GLuint getShader();

	virtual void useShader();
	virtual void stopShader();
	virtual void update();
	//virtual void update();

	void activateAlphaBlend();
	void deactivateAlphaBlend();

	void setEntity(Entity* e);
	void reloadShader();
private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
	Entity* entity;
	std::string filename;
};

