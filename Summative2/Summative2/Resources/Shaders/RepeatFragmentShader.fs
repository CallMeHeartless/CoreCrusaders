#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform float fXFrameWidth;
uniform float fYFrameHeight;
uniform int iXIndex;
uniform int iYIndex;
uniform sampler2D tex;

void main(void){
	
	color = texture(tex, fragTexCoord) * vec4(fragColor, 1.0f);
}