#version 330 core

layout (location = 0) in vec2 inFragPosition;
layout (location = 1) in vec2 inTextureCoord;

uniform mat4 projection;
uniform vec2 uOffset;
uniform vec4 uColor;
uniform float uScale = 10.0;

out vec2 fTextureCoord;
out vec4 fParticleColor;

void main()
{
	fTextureCoord = inTextureCoord;
	fParticleColor = uColor;
	gl_Position = projection * vec4((inFragPosition * uScale) + uOffset, 0.0, 1.0);
}