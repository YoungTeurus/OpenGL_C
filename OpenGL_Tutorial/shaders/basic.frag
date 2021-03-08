#version 330 core

in vec4 fColor;
out vec4 outColor;

void main()
{
	outColor = fColor;
	//outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}