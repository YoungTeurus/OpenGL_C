#version 330 core

#define MAX_LIGHTS 8

struct Material {
	sampler2D	diffuse;	// ����� ��������� (��������)
	sampler2D	specular;	// ����� ��� ������������ ����� (��������)

	sampler2D	emission;	// ����� ��������

	float		shininess;	// ����������� �����������
};

struct Light{
	int type;

	vec3 position;

	// ��� Direction � Spot ����������:
	vec3 direction;

	// ��� Spot ���������:
	float cutOff;

	// ���� ���������:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// ��� ���������� ����������:
	float constant;
	float linear;
	float quadratic;
};

in vec3 fNormal;
in vec3 fFragPosition;
in vec2 fTextureCoord;

in mat3 fTBN;

out vec4 fragColor;

uniform Material material;

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


void main(){
	vec3 currentLightColorResult;  // ��������� ������ ��� �������� ������������� ����� ��� �������� ��������� �����
	for(int i = 0; i < lightsCount; i++){
		if (lights[i].type == 1){ // ������������ ����:
			// ������ ����������� � ��������� ����� (���������������, ��� ������� ��������������� ������):
			vec3 lightDirection = -lights[i].direction;

			// ���� ������������� �������:
			vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

			// ���� ���������� �� ���������:
			vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, lightDirection);

			currentLightColorResult = ambientColor + diffuseAndSpecularColor;
		} else {
			// ��������������� ������ ����������� �� ����� ������� � ��������� �����:
			vec3 fromCollisionPointToLightDirection = normalize(lights[i].position - fFragPosition);
			if( lights[i].type == 2) {  // �������� �������� �����:
				// ������� ��������� �����:
				float attenuation = getAttenuation(i);

				// ���� ������������� �������:
				vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

				// ���� ���������� �� ���������:
				vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, fromCollisionPointToLightDirection);

				currentLightColorResult = (ambientColor + diffuseAndSpecularColor) * attenuation;
			} else if (lights[i].type == 3) {  // ������������ �������� �����:
				// ���� ����� ���������������� �������� ����������� � ��������� ����� � �������� ����������� �� ����� ������� � ��������� �����:
				float angle = acos(dot(fromCollisionPointToLightDirection, normalize(-lights[i].direction)));

				// ���� ����� ���� �� ����� ������:
				if (angle <= lights[i].cutOff * 2.0f){
					// ����������� ����������� ������� ���������:
					float koef = 1.0f;
					
					if (angle >= lights[i].cutOff){
						// ��������� koef � ��������� [0;1] � ����������� �� ����:
						koef = (lights[i].cutOff*2.0f - angle) / lights[i].cutOff;
					}

					// ������� ��������� �����:
					float attenuation = getAttenuation(i);

					// ���� ������������� �������:
					vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

					// ���� ���������� �� ���������:
					vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, fromCollisionPointToLightDirection) * koef;

					currentLightColorResult = (ambientColor + diffuseAndSpecularColor) * attenuation;
				} else {  // angle > lights[i].cutOff * 2.0f
					// ���� ������������� �������:
					currentLightColorResult = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;
				}
			}
		}
		fragColor += vec4(currentLightColorResult, 1.0f);
	}
}

float getAttenuation(int i){
	float dist = distance(lights[i].position, fFragPosition);
	float attenuation = 1.0 / (lights[i].constant + lights[i].linear*dist + lights[i].quadratic * dist * dist);
	return attenuation;
}

vec3 getDiffuseAndSpecularColor(int i, vec3 lightDirection){
	// �������� �������� ������� � ��������� [0;1] :
	vec3 normal = texture(texture_normal1, fTextureCoord).rgb;
	// �������� ������� � ��������� [-1;1] :
	normal = normal * 2.0f - 1.0f;
	// �������� ������� � ��������� [-1;1] :
	normal = normalize(normal);
	// ?!
	normal = normalize(TBN * normal);


	// ��������� ������������ ��������������� �������� ������� � ����������� � ��������� �����. ����� ���� � ��������� [-1;1] :
	float angleDot = dot(normal, lightDirection);
	// ������� ���������� ���������: [0;1]
	float diffuseKoeff = max(angleDot, 0.0);
	vec3 diffuseColor = lights[i].diffuse * diffuseKoeff * texture(texture_diffuse1, fTextureCoord).rgb;

	// ����������� ���������� �����:
	vec3 reflectDirection = reflect(lightDirection, normal);
	// ����������� �� ��������� ������ � ����� �������:
	vec3 viewDirection = normalize(fragPosition - viewPos);
	// ��������� ������������ �������� ����������� ������� � ���������� ���� [-1;1]:
	float viewAndReflectDirectionsDotProduct = dot(viewDirection, reflectDirection);

	// viewAndReflectDirectionsDotProduct �������������?! (��������� ������ ������������ ������� ������)

	// �������� ������������ �����:
	float specularKoeff = pow(max(viewAndReflectDirectionsDotProduct, 0.0f), shininess);
	vec3 specularColor = lights[i].specular * specularKoeff * texture(texture_specular1, fTextureCoord).rgb;

	return diffuseColor + specularColor;
}