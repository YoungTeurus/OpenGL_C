#version 330 core
in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D hdrBuffer;
uniform float exposure = 1.0;

void main(){
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrBuffer, fTexCoords).rgb;

	// Коррекция тона Reinhard-а (перевод HDR в SDR):
	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

	// Гамма-коррекция (поправка яркости для соответствия человеческому глазу):
	mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped, 1.0);
}