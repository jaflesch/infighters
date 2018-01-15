#version 330 core

in vec2 uvCoords;
out vec4 out_Color;

uniform sampler2D TextureSampler;

void main(){
	out_Color = texture(TextureSampler, uvCoords);
	//out_Color = vec4(1.0, 1.0, 1.0, 1.0);
	//out_Color = vec4(uvCoords, 0.0, 1.0);
}