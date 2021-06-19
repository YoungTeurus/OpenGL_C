#version 330 core

layout (location = 0) in vec2 inFragPosition;
layout (location = 1) in vec2 inTextureCoord;

uniform mat4 projection;

out vec2 fTextureCoord;

void main()
{
	gl_Position = projection * vec4(inFragPosition, 0.0, 1.0);
	fTextureCoord = inTextureCoord;
}