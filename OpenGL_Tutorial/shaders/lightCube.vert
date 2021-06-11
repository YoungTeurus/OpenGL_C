#version 330 core

layout (location = 0) in vec3 inFragPosition;

out vec4 fColor;

uniform mat4 model;
uniform mat4 projectionAndView;

void main()
{
	gl_Position = projectionAndView * model * vec4(inFragPosition, 1.0);
}