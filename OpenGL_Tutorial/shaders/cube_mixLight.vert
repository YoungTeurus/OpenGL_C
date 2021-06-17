#version 330 core

layout (location = 0) in vec3 inFragPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoord;

out vec3 fNormal;
out vec3 fFragPosition;
out vec2 fTextureCoord;

uniform mat4 projectionAndView;
uniform mat4 model;

void main(){
	vec4 vertPosition = model * vec4(inFragPosition, 1.0);
	gl_Position = projectionAndView * vertPosition;

	fNormal = normalize(mat3(model) * inNormal);
	fTextureCoord = inTextureCoord;
}
