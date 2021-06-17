#version 330 core

#define MAX_LIGHTS 8

struct Light{
	int type;

	vec3 position;

	// ��� Direction � Spot ����������:
	vec3 direction;

	// ��� Spot ���������:
	float cutOffCosin;
	float outerCutOffCosin;

	// ���� ���������:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// ��� ���������� ����������:
	float constant;
	float linear;
	float quadratic;
};

int LightDirectionalType = 1;
int LightPointType = 2;
int LightSpotType = 3;
int LightAmbientType = 4;

in vec3 gNormal;
in vec3 gFragPosition;
in vec2 gTextureCoord;
in mat3 gTBN;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform int lightsCount;
uniform Light lights[MAX_LIGHTS];

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess;

uniform vec3 viewPos;  // ��������� ������ � ������� �����������

// ���������� �������:

// ����������� ����� � ��������� [0;1], ���������� ������������� ������� ����� ��������� �����.
// ����������� � ����������� ���������� �� ��������� �����.
float getAttenuation(int i);
// ����������� ������������ ���� (��������) ��� ���������� � ������������ ��������� ��� i-��� ��������� �����.
// ���� ��������� � ����������� lightDirection �� �������.
vec3 getDiffuseAndSpecularColor(int i, vec3 lightDirection);

vec3 getColorFromLight(Light light, vec3 normal, vec3 fFragPosition, vec3 viewDir, vec3 diffuseTextureColor, vec3 specular);
vec3 getColorFromDirLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);
vec3 getColorFromPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);
vec3 getColorFromSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);

void main(){
	vec3 normal, diffuseTextureColor, specularTextureColor, viewDir;

	normal = texture(texture_normal1, gTextureCoord).rgb;
	normal = normal * 2.0f - 1.0f;
	normal = normalize(gTBN * normal);
	diffuseTextureColor = texture(texture_diffuse1, gTextureCoord).rgb;
	specularTextureColor = texture(texture_specular1, gTextureCoord).rgb;
	viewDir = normalize(viewPos - gFragPosition);

	vec3 result = vec3(0, 0, 0);

	for(int i = 0; i < lightsCount; i++){
		result += getColorFromLight(lights[i], normal, gFragPosition, viewDir, diffuseTextureColor, specularTextureColor);
	}

	fragColor = vec4(result, 1.0);

	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0){
		brightColor = vec4(fragColor.rgb, 1.0);
	} else {
		brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

vec3 getColorFromLight(Light light, vec3 normal, vec3 fFragPosition, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
	if (light.type == LightDirectionalType){
		return getColorFromDirLight(light, normal, viewDir, diffuseTextureColor, specularTextureColor);
	} else if (light.type == LightPointType){
		return getColorFromPointLight(light, normal, fFragPosition, viewDir, diffuseTextureColor, specularTextureColor);
	} else if (light.type == LightSpotType){
		return getColorFromSpotLight(light, normal, fFragPosition, viewDir, diffuseTextureColor, specularTextureColor);
	} else if (light.type == LightAmbientType){
		return vec3(0, 0, 0);
	}
}

vec3 getColorFromDirLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
	vec3 lightDir = normalize(-light.direction); // ������ ����������� �����

	// ��������� ������������, ��� ������� ���� [0,1], ��� ��� ��� ������� �������������
	// diff = 1, ����� ���� = 0 (���� ������ ���������������)
	float diff = max( dot(normal, lightDir), 0.0);

	// �������� �������� ���� ������������ ������� �������
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - ������� ����������� �����������. ��� ������, ��� ����� ����� �������� ������� �����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	// ����������� ����������:
	vec3 _ambient = light.ambient * diffuseTextureColor;
	vec3 _diffuse = light.diffuse * diff * diffuseTextureColor;
	vec3 _specular = light.specular * spec * specularTextureColor;
	return (_ambient + _diffuse + _specular);
}

vec3 getColorFromPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specularMap = specularTextureColor;
	
	vec3 ambient = light.ambient * diffuseTextureColor * attenuation;
	vec3 diffuse = light.diffuse * diff * diffuseTextureColor * attenuation;
	vec3 specular = light.specular * spec * specularMap * attenuation;
	return (ambient + diffuse + specular);
}

vec3 getColorFromSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light.ambient * diffuseTextureColor * intensity_angle * attenuation;
	vec3 diffuse = light.diffuse * diff * diffuseTextureColor * intensity_angle * attenuation;
	vec3 specular = light.specular * spec * specularTextureColor * intensity_angle * attenuation;
	return (ambient + diffuse + specular);
}