#include "AnimationShader.h"
#include "font_render/os.h"

extern Window_Info window_info;

AnimationShader::AnimationShader(std::string fileName) : Shader(fileName)
{
	getUniformLocations();
}

AnimationShader::~AnimationShader() {}

void AnimationShader::getUniformLocations()
{
	uniformTexture = glGetUniformLocation(shader, "TextureSampler");
	uniformProjection = glGetUniformLocation(shader, "projection_m");
	uniformTextureNumRows = glGetUniformLocation(shader, "textureNumRows");
	uniformTextureOffset = glGetUniformLocation(shader, "textureOffset");
}

void AnimationShader::update(hm::vec2& translation, Animation* animation)
{
	glBindTexture(GL_TEXTURE_2D, animation->m_texture->textureID);

	float x = roundf(translation.x * 2.0f);
	float y = roundf(-(translation.y * 2.0f));

	float windowWidth = (float)window_info.width;
	float windowHeight = (float)window_info.height;

	//glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(x, y, 0));
	hm::mat4 model = hm::mat4::translate(hm::vec3(x, y, 0));

	//glm::mat4 ortho = glm::ortho(-1.0f * 16 / 9, 1.0f * 16 / 9, -1.0f, 1.0f);
	hm::mat4 ortho = hm::mat4::ortho(-windowWidth, windowWidth, -windowHeight, windowHeight);

	hm::mat4 finalMatrix = ortho * model;
	hm::transpose(finalMatrix);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, (GLfloat*)&finalMatrix.data);

	glUniform1i(uniformTexture, 0);

	//hm::vec2 rows(5.0f, 2.0f);
	
	glUniform2fv(uniformTextureNumRows, 1, &animation->m_rows_cols.x);
	//hm::vec2 offset = hm::vec2(0, 0);
	glUniform2fv(uniformTextureOffset, 1, &animation->m_offset.x);
}

void AnimationShader::bindTextures(GLuint textureId)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void AnimationShader::unbindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}