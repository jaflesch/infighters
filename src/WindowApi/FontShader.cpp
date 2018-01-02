#include "FontShader.h"

#include <hmath.h>

#include <fstream>
#include <iostream>

FontShader::FontShader(std::string filename) : Shader(filename)
{
	getUniformLocations();
}

FontShader::~FontShader()
{
}

void FontShader::getUniformLocations()
{
	uniformTextTexture = glGetUniformLocation(shader, "textTexture");
	uniformTextColor = glGetUniformLocation(shader, "textColor");

	uniformClipTopLeft = glGetUniformLocation(shader, "clipTL");
	uniformClipBotRight = glGetUniformLocation(shader, "clipBR");
}

void FontShader::update()
{
	glUniform4fv(uniformTextColor, 1, &m_textColor.x);
	glUniform1i(uniformTextTexture, 0);
	glUniform2fv(uniformClipTopLeft, 1, &clipTL.x);
	glUniform2fv(uniformClipBotRight, 1, &clipBR.x);
}