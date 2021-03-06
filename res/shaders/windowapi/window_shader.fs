#version 330 core

in vec4 position;
in vec2 uvCoords;

out vec4 out_Color;

uniform vec4 backgroundColor;

uniform vec2 clipTL;
uniform vec2 clipBR;
uniform float opacity;

uniform int useTexture;
uniform sampler2D textureSampler;

void main(){
	if(position.x < clipTL.x || position.x > clipBR.x)
		discard;
	if(position.y > -clipTL.y || position.y < -clipBR.y)
		discard;

	if(useTexture == 1)
	{
		vec4 textureColor = texture(textureSampler, uvCoords);
		//float finalAlpha = textureColor.a - 1 + backgroundColor.a;
		vec3 finalColor = mix(backgroundColor.rgb, textureColor.rgb, vec3(backgroundColor.a));
		out_Color = vec4(finalColor, textureColor.a * opacity);
	}
	else{
		out_Color = backgroundColor;
	}
}