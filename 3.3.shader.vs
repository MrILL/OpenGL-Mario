#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float x = 0;
uniform float y = 0;

uniform float halfH = 0;
uniform float halfW = 0;

void main()
{
	gl_Position = vec4(
		-1 + x + aPos.x + halfW, 
		-1 + y + aPos.y + halfH, 
		aPos.z, 
		1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}