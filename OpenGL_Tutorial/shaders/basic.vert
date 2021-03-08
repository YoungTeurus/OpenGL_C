#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;

out vec4 fColor;

void main()
{
	gl_Position = vec4(inPos.xyz, 1.0f);
	fColor = inColor;
}