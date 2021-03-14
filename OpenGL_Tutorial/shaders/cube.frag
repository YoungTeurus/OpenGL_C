#version 330 core

in vec3 fNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;  // ��������� ������ � ������� �����������

void main(){
	float ambientStrength = 0.1;
	float specularStrength = 0.5;

	// ������� ���������:
	vec3 ambient = ambientStrength * lightColor;

	// ������������ ����:
	vec3 norm = normalize(fNormal);  // ������� � �����������
	vec3 lightDir = normalize(lightPos - fragPosition); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = diff * lightColor;

	// Specular ����:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, norm);

	// 32 - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(result, 1.0);
}