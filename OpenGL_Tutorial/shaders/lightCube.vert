#version 330 core

layout (location = 0) in vec3 inFragPosition;
layout (location = 1) in vec3 inNormal;

out vec4 fColor;
out vec3 fFragPosition;  // ѕоложение точки в мировых координатах

uniform mat4 model;
uniform mat4 perspectiveAndView;

void main()
{
	vec4 vertPosition = model * vec4(inFragPosition, 1.0);
	gl_Position = perspectiveAndView * vertPosition;
}