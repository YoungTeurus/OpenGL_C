#version 330 core

layout (location = 0) in vec2 inFragPosition;
layout (location = 1) in vec2 inTexCoords;

out vec2 fTexCoords;

void main()
{
	gl_Position = vec4(inFragPosition.x, inFragPosition.y, 0.0, 1.0);
	fTexCoords = inTexCoords;
}