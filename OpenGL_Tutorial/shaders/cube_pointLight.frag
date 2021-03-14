#version 330 core

struct Material {
	sampler2D	diffuse;	// ����� ��������� (��������)
	sampler2D	specular;	// ����� ��� ������������ ����� (��������)

	sampler2D	emission;	// ����� ��������

	float		shininess;	// ����������� �����������
};

struct Light {
	vec3 position;		// ����������������� ��������� �����
	
	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����

	float constant;
	float linear;
	float quadratic;
};

in vec3 fNormal;
in vec3 fragPosition;
in vec2 fTextureCoord;

out vec4 fragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;  // ��������� ������ � ������� �����������

void main(){
	vec3 fragToLight = light.position - fragPosition;  // ������, ������������ �� ������� �������� � ��������� �����
	float distanceToLight = length(fragToLight);
	// "��������� �����" - �������� ���������� �� ���������� �� ��������� �����
	float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

	// ������� ���������:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));
	ambient *= attenuation;

	// ������������ ����:
	vec3 norm = normalize(fNormal);  // ������� � �����������
	vec3 lightDir = normalize(fragToLight); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord));
	diffuse *= attenuation;

	// Specular ����:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, norm);

	// shininess - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));
	vec3 specular = light.specular * spec * specularMap;
	specular *= attenuation;

	// �������� �������
	vec3 emission = vec3(texture(material.emission, fTextureCoord)) * (vec3(1.0) * int(specularMap.r == 0));

	vec3 result = ambient + diffuse + specular + emission;

	fragColor = vec4(result, 1.0);
}