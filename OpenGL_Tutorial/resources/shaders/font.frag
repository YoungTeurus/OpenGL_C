#version 330 core
in vec2 fTextureCoord;
out vec4 fragColor;

uniform sampler2D textTexture;
uniform vec3 uTextColor;

void main(){
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, fTextureCoord).r);
    fragColor = vec4(uTextColor, 1.0) * sampled;
}