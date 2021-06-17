#version 330 core

in vec3 fTexCoords;  // Направление вектора "взгляда"
uniform samplerCube cubemap;

out vec4 fragColor;

void main(){
	fragColor = texture(cubemap, fTexCoords);
}