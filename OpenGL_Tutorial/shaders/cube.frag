#version 330 core

struct Material {
	sampler2D	diffuse;	// ����� ��������� (��������)
	sampler2D	specular;	// ����� ��� ������������ ����� (��������)
	float		shininess;	// ����������� �����������
};

struct Light {
	vec3 position;		// ����������������� ��������� �����

	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����
};

in vec3 fNormal;
in vec3 fragPosition;
in vec2 fTextureCoord;

out vec4 fragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;  // ��������� ������ � ������� �����������

void main(){
	// ������� ���������:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));

	// ������������ ����:
	vec3 norm = normalize(fNormal);  // ������� � �����������
	vec3 lightDir = normalize(light.position - fragPosition); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord));

	// Specular ����:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, norm);

	// 32 - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * spec * vec3(texture(material.specular, fTextureCoord));

	vec3 result = ambient + diffuse + specular;

	fragColor = vec4(result, 1.0);
}