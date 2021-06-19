#version 330 core

layout (location = 0) in vec3 inFragPosition;
layout (location = 2) in vec2 inTextureCoord;

uniform mat4 model;
uniform mat4 pv;
uniform vec4 uColor;
uniform float uScale = 0.025;

out vec2 fTextureCoord;
out vec4 fParticleColor;

void main()
{
	fTextureCoord = inTextureCoord;
	fParticleColor = uColor;
	gl_Position = pv* model * vec4((inFragPosition * uScale), 1.0);
}