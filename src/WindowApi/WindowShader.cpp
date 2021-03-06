#include "WindowShader.h"
#include "../font_render/os.h"

extern Window_Info window_info;

WindowShader::WindowShader(std::string fileName) : Shader(fileName)
{
	m_opacity = 1.0f;
	getUniformLocations();
}

WindowShader::~WindowShader(){}

void WindowShader::getUniformLocations()
{
	uniformUseTexture = glGetUniformLocation(shader, "useTexture");
	uniformTexture = glGetUniformLocation(shader, "textureSampler");
	uniformTextColor = glGetUniformLocation(shader, "backgroundColor");
	uniformProjection = glGetUniformLocation(shader, "Model");

	uniformClipTopLeft = glGetUniformLocation(shader, "clipTL");
	uniformClipBotRight = glGetUniformLocation(shader, "clipBR");

	uniformOpacity = glGetUniformLocation(shader, "opacity");
}

void WindowShader::update(hm::vec2& translation)
{
	float x = roundf(translation.x * 2.0f);
	float y = roundf(-(translation.y * 2.0f));

	float windowWidth = (float)window_info.width;
	float windowHeight = (float)window_info.height;

	//glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(x, y, 0));
	hm::mat4 model = hm::mat4::translate(hm::vec3(x, y, 0));

	//glm::mat4 ortho = glm::ortho(-1.0f * 16 / 9, 1.0f * 16 / 9, -1.0f, 1.0f);
	hm::mat4 ortho = hm::mat4::ortho(-windowWidth, windowWidth, -windowHeight, windowHeight);

	hm::mat4 finalMatrix = ortho * model;
	glUniform4fv(uniformTextColor, 1, &m_backgroundColor.x);
	hm::transpose(finalMatrix);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, (GLfloat*)&finalMatrix.data);

	glUniform2fv(uniformClipTopLeft, 1, &clipTL.x);
	glUniform2fv(uniformClipBotRight, 1, &clipBR.x);

	glUniform1f(uniformOpacity, m_opacity);

	glUniform1i(uniformTexture, 0);
	glUniform1i(uniformUseTexture, useTexture);
}

void WindowShader::bindTextures(GLuint textureId)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	useTexture = 1;
}

void WindowShader::unbindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	useTexture = 0;
}