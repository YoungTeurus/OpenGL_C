#version 330 core

struct Material {
	vec3 ambient;		// ���� ������� � ������� ���������
	vec3 diffuse;		// ���� ������� � ��������� ���������
	vec3 specular;		// ���� ������� � ����������� ���������
	float shininess;	// ����������� �����������
};

struct Light {
	vec3 position;		// ����������������� ��������� �����

	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����
};

in vec3 fNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;  // ��������� ������ � ������� �����������

void main(){
	// ������� ���������:
	vec3 ambient = material.ambient * light.ambient;

	// ������������ ����:
	vec3 norm = normalize(fNormal);  // ������� � �����������
	vec3 lightDir = normalize(light.position - fragPosition); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	// Specular ����:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, norm);

	// 32 - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = (material.specular * spec) * light.specular;

	vec3 result = ambient + diffuse + specular;

	fragColor = vec4(result, 1.0);
}