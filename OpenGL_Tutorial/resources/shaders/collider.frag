#version 330 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main(){
	fragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f);

	brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}