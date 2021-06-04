#version 330 core

#define NUMBER_OF_MAX_POINT_LIGHTS 8

struct Material {
	sampler2D	diffuse;	// ����� ��������� (��������)
	sampler2D	specular;	// ����� ��� ������������ ����� (��������)

	sampler2D	emission;	// ����� ��������

	float		shininess;	// ����������� �����������
};

// ������������ ����
struct DirLight {
	vec3 direction; 

	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����
};

// �������� ����
struct PointLight {
	vec3 position;

	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����

	// ���������� ��� �������� ������������� �����:
	float constant;
	float linear;
	float quadratic;
};

// �������� ����-�������
struct SpotLight{
	vec3 position;
	vec3 direction;		// ����������� �����
	float cutOffCosin;	// ������� ���� ��������������� ����� �� 100% ��������������
	float outerCutOffCosin;	// ������� ���� ��������������� �����, �� ������� ������������� = 0%

	vec3 ambient;		// ���� (����?) �������� �����
	vec3 diffuse;		// ���� (����?) ���������� �����
	vec3 specular;		// ���� (����?) ������������ �����
	
	// ���������� ��� �������� ������������� �����:
	float constant;
	float linear;
	float quadratic;
};

// ������������ ������� ������������� ����� �� ������� � ��������� �������� � ������������ �������
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// ������������ ������� ��������� ����� �� ������� � ��������� �������� � ������������ �������
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// ������������ ������� spotlight ����� �� ������� � ��������� �������� � ������������ �������
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


in vec3 fNormal;
in vec3 fragPosition;
in vec2 fTextureCoord;

out vec4 fragColor;

uniform Material material;
uniform bool hasDirLight = false;
uniform DirLight dirLight;
uniform int pointLightsNumber = 0;
uniform PointLight pointLights[NUMBER_OF_MAX_POINT_LIGHTS];
uniform bool hasSpotLight = false;
uniform SpotLight spotLight;

uniform vec3 viewPos;  // ��������� ������ � ������� �����������

void main(){
	vec3 normal = normalize(fNormal);  // ���������� � �����
	vec3 viewDir = normalize(viewPos - fragPosition);  // ���� ����� �������� � ���������

	vec3 result = vec3(0.0, 0.0, 0.0);

	// 1. �� ������ ������ ������������ ���������
	if (hasDirLight){
		result += CalcDirLight(dirLight, normal, viewDir);
	}
	// 2. ����������� ����� �� �������� ����������
	for(int i = 0; i < pointLightsNumber; i++){
		result += CalcPointLight(pointLights[i], normal, fragPosition, viewDir);
	}
	// 3. ����������� ����� �� ��������
	if (hasSpotLight){
		result += CalcSpotLight(spotLight, normal, fragPosition, viewDir);
	}

	// 4. ����������� ������������ ��������
	vec3 emission = vec3(texture(material.emission, fTextureCoord));
	result += emission;

	fragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(normal, lightDir), 0.0);

	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// ����������� ����������:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, fTextureCoord));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 fragToLight = light.position - fragPos;  // ������, ������������ �� ������� �������� � ��������� �����
	vec3 lightDir = normalize(fragToLight); // ������ ����������� �����
	float distanceToLight = length(fragToLight);
	// "��������� �����" - �������� ���������� �� ���������� �� ��������� �����
	float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(normal, lightDir), 0.0);

	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord)) * attenuation;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord)) * attenuation;
	vec3 specular = light.specular * spec * specularMap * attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 vertToLightVec = light.position - fragPos;  // ������, ������������ �� ������� �������� � ��������� �����
	float vertToLightDistance = length(vertToLightVec);

	// "��������� �����" - �������� ���������� �� ���������� �� ��������� �����
	float attenuation = 1.0 / (light.constant + light.linear * vertToLightDistance + light.quadratic * vertToLightDistance * vertToLightDistance);

	// ����������� �� �������� � �����
	vec3 lightDir = normalize(vertToLightVec); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(normal, lightDir), 0.0);
	
	// ������� ������� ���� ����� ������������ ������ � ������������ �� ������� � �����
	float cosinBetweenLightAndVertex = dot(lightDir, normalize(-light.direction));
	// ���������� ��������: I = (0 - y) / e (https://learnopengl.com/Lighting/Light-casters) , ����� ������� ������� ������� �����
	float difference = light.cutOffCosin - light.outerCutOffCosin;
	float intensity_angle = clamp((cosinBetweenLightAndVertex - light.outerCutOffCosin)/difference, 0.0, 1.0);

	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord)) * intensity_angle * attenuation;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord)) * intensity_angle * attenuation;
	vec3 specular = light.specular * spec * specularMap * intensity_angle * attenuation;
	return (ambient + diffuse + specular);
}