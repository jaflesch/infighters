#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;

uniform mat4 projection_m;
uniform vec2 textureNumRows;
uniform vec2 textureOffset;

void main()
{
	gl_Position = projection_m * vec4(vertexPosition_modelspace, 1.0);
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	//textureOffset = textureOffset / textureNumRows;
	//uvCoords = (uvCoords/textureNumRows) + textureOffset;
	uvCoords = (uvCoords/textureNumRows) + vec2(textureOffset.x / textureNumRows.x, textureOffset.y / textureNumRows.y);
}