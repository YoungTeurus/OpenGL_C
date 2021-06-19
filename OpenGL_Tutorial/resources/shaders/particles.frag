#version 330 core

in vec2 fTextureCoord;
in vec4 fParticleColor;

out vec4 fragColor;

uniform sampler2D sprite;

void main(){
	fragColor = (texture(sprite, fTextureCoord) * fParticleColor);
}
