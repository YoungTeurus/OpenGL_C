#version 330 core

#define MAX_LIGHTS 8

struct Light{
	int type;

	vec3 position;

	// Для Direction и Spot источников:
	vec3 direction;

	// Для Spot источника:
	float cutOffCosin;
	float outerCutOffCosin;

	// Цвет источника:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Для затухающих источников:
	float constant;
	float linear;
	float quadratic;
};

int LightDirectionalType = 1;
int LightPointType = 2;
int LightSpotType = 3;
int LightAmbientType = 4;

in vec3 fNormal;
in vec3 fFragPosition;
in vec2 fTextureCoord;

in mat3 fTBN;

out vec4 fragColor;

uniform int lightsCount;
uniform Light lights[MAX_LIGHTS];

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess;

uniform vec3 viewPos;  // Положение камеры в мировых координатах

// Объявление функций:

// Вовзвращает число в диапазоне [0;1], отражающее относительную яркость света источника света.
// Уменьшается с увеличением расстояния до источника света.
float getAttenuation(int i);
// Вовзвращает подсчитанный цвет (текстуры) для диффузного и спекулярного освещения для i-ого источника света.
// Свет находится в направлении lightDirection от объекта.
vec3 getDiffuseAndSpecularColor(int i, vec3 lightDirection);

void calculateNormalDiffuseSpecualarAndViewDirVectors(vec3 normal, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 viewDir);

vec3 getColorFromLight(Light light, vec3 normal, vec3 fFragPosition, vec3 viewDir, vec3 diffuseTextureColor, vec3 specular);
vec3 getColorFromDirLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);
vec3 getColorFromPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);
vec3 getColorFromSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor);

void main(){
	vec3 normal, diffuseTextureColor, specularTextureColor, viewDir;

	// calculateNormalDiffuseSpecualarAndViewDirVectors(normal, diffuse, specular, viewDir);
	// {
	normal = texture(texture_normal1, fTextureCoord).rgb;
	normal = normal * 2.0f - 1.0f;
	normal = normalize(fTBN * normal);
	diffuseTextureColor = texture(texture_diffuse1, fTextureCoord).rgb;
	specularTextureColor = texture(texture_specular1, fTextureCoord).rgb;
	viewDir = normalize(viewPos - fFragPosition);
	// }

	vec3 result = vec3(0, 0, 0);

	for(int i = 0; i < lightsCount; i++){
		result += getColorFromLight(lights[i], normal, fFragPosition, viewDir, diffuseTextureColor, specularTextureColor);
	}

	fragColor = vec4(result, 1.0);
}

void calculateNormalDiffuseSpecualarAndViewDirVectors(vec3 normal, vec3 diffuse, vec3 specular, vec3 viewDir){
	// Получаем значение нормали в диапазоне [0;1] :
	normal = texture(texture_normal1, fTextureCoord).rgb;
	// Получаем нормаль в диапазоне [-1;1] :
	normal = normal * 2.0f - 1.0f;
	// ?!
	normal = normalize(fTBN * normal);

	diffuse = texture(texture_diffuse1, fTextureCoord).rgb;
	specular = texture(texture_specular1, fTextureCoord).rgb;
	viewDir = normalize(viewPos - fFragPosition);
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
	vec3 lightDir = normalize(-light.direction); // Вектор направления света

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(normal, lightDir), 0.0);

	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	// Комбинируем результаты:
	vec3 _ambient = light.ambient * diffuseTextureColor;
	vec3 _diffuse = light.diffuse * diff * diffuseTextureColor;
	vec3 _specular = light.specular * spec * specularTextureColor;
	return (_ambient + _diffuse + _specular);
}

vec3 getColorFromPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
	vec3 fragToLight = light.position - fragPos;  // Вектор, направленный из позиции вертекса к источнику света
	vec3 lightDir = normalize(fragToLight); // Вектор направления света
	float distanceToLight = length(fragToLight);
	// "Затухание света" - поправка светимости от расстояния до источника света
	float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(normal, lightDir), 0.0);

	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specularMap = specularTextureColor;
	
	vec3 ambient = light.ambient * diffuseTextureColor * attenuation;
	vec3 diffuse = light.diffuse * diff * diffuseTextureColor * attenuation;
	vec3 specular = light.specular * spec * specularMap * attenuation;
	return (ambient + diffuse + specular);
}

vec3 getColorFromSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTextureColor, vec3 specularTextureColor){
	vec3 vertToLightVec = light.position - fragPos;  // Вектор, направленный из позиции вертекса к источнику света
	float vertToLightDistance = length(vertToLightVec);

	// "Затухание света" - поправка светимости от расстояния до источника света
	float attenuation = 1.0 / (light.constant + light.linear * vertToLightDistance + light.quadratic * vertToLightDistance * vertToLightDistance);

	// Направление от вертекса к свету
	vec3 lightDir = normalize(vertToLightVec); // Вектор направления света

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(normal, lightDir), 0.0);
	
	// Находим косинус угла между направлением фонаря и направлением от объекта к свету
	float cosinBetweenLightAndVertex = dot(lightDir, normalize(-light.direction));
	// Используем форумулу: I = (0 - y) / e (https://learnopengl.com/Lighting/Light-casters) , чтобы сделать плавную границу света
	float difference = light.cutOffCosin - light.outerCutOffCosin;
	float intensity_angle = clamp((cosinBetweenLightAndVertex - light.outerCutOffCosin)/difference, 0.0, 1.0);

	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light.ambient * diffuseTextureColor * intensity_angle * attenuation;
	vec3 diffuse = light.diffuse * diff * diffuseTextureColor * intensity_angle * attenuation;
	vec3 specular = light.specular * spec * specularTextureColor * intensity_angle * attenuation;
	return (ambient + diffuse + specular);
}