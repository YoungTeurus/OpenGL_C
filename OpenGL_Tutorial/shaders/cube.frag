#version 330 core

in vec3 fNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// ������������ ����:
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fragPosition); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(norm, lightDir) ,0.0);

	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;

	fragColor = vec4(result, 1.0);
}