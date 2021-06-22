#version 330 core

layout (location = 0) in vec3 inFragPosition;

uniform mat4 projectionAndView;

void main()
{
	gl_Position = projectionAndView * vec4(inFragPosition, 1.0);
}