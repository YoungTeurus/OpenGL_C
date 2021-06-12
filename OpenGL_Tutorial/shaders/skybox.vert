#version 330 core

layout (location = 0) in vec3 inFragPosition;

uniform mat4 view;
uniform mat4 projection;

out vec3 fTexCoords;

void main()
{
	fTexCoords = inFragPosition;
	vec4 position = projection * view * vec4(inFragPosition, 1.0);
	gl_Position = position.xyww;  // z всегда равен 1.0 (w / w)
}