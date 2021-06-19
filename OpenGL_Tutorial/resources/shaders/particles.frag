#version 330 core

in vec2 fTextureCoord;
in vec4 fParticleColor;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main(){
	fragColor = fParticleColor;

	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0){
		brightColor = vec4(fragColor.rgb, 1.0);
	} else {
		brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
