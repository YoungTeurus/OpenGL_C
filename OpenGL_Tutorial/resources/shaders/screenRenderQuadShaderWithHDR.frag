#version 330 core
in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D hdrBuffer;
uniform sampler2D blurBuffer;
uniform float exposure = 1.0;

void main(){
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrBuffer, fTexCoords).rgb;
	vec3 bloomColor = texture(blurBuffer, fTexCoords).rgb;

	hdrColor += bloomColor;

	// ��������� ���� Reinhard-� (������� HDR � SDR):
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	// �����-��������� (�������� ������� ��� ������������ ������������� �����):
	result = pow(result, vec3(1.0 / gamma));

	fragColor = vec4(result, 1.0);
}