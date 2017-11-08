#include "WindowShader.h"

extern Window_State win_state;

WindowShader::WindowShader(std::string fileName) : Shader(fileName, nullptr)
{
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
}

void WindowShader::update(hm::vec2& translation)
{
	float x = translation.x * 2.0f;
	float y = -(translation.y * 2.0f);

	float windowWidth = win_state.win_width;
	float windowHeight = win_state.win_height;

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