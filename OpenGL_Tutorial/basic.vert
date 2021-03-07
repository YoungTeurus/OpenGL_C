#version 330 core

layout (location = 0) in vec3 fPos;
layout (location = 1) in vec3 inColor;

out vec3 fColor;

void main()
{
	gl_Position = vec4(fPos.xyz, 1.0f);
	fColor = inColor;
}