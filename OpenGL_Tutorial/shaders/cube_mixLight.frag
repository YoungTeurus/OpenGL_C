#version 330 core

#define NUMBER_OF_MAX_POINT_LIGHTS 8

struct Material {
	sampler2D	diffuse;	// Карта освещения (текстура)
	sampler2D	specular;	// Карта для спекулярного света (текстура)

	sampler2D	emission;	// Карта свечения

	float		shininess;	// Глянцевость поверхности
};

// Направленный свет
struct DirLight {
	vec3 direction; 

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света
};

// Точечный свет
struct PointLight {
	vec3 position;

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света

	// Переменные для убывания интенсивности света:
	float constant;
	float linear;
	float quadratic;
};

// Конусный свет-фонарик
struct SpotLight{
	vec3 position;
	vec3 direction;		// Направление света
	float cutOffCosin;	// Косинус угла распространения света со 100% интенсивностью
	float outerCutOffCosin;	// Косинус угла распространения света, за которым интенсивность = 0%

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света
	
	// Переменные для убывания интенсивности света:
	float constant;
	float linear;
	float quadratic;
};

// Рассчитывает влияние направленного света на вертекс с указанной нормалью и направлением взгляда
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// Рассчитывает влияние точечного света на вертекс с указанной нормалью и направлением взгляда
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// Рассчитывает влияние spotlight света на вертекс с указанной нормалью и направлением взгляда
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

uniform vec3 viewPos;  // Положение камеры в мировых координатах

void main(){
	vec3 normal = normalize(fNormal);  // Нормальная к грани
	vec3 viewDir = normalize(viewPos - fragPosition);  // Угол между взглядом и вертексом

	vec3 result = vec3(0.0, 0.0, 0.0);

	// 1. За основу берётся направленное освещение
	if (hasDirLight){
		result += CalcDirLight(dirLight, normal, viewDir);
	}
	// 2. Прибавление света от точечных источников
	for(int i = 0; i < pointLightsNumber; i++){
		result += CalcPointLight(pointLights[i], normal, fragPosition, viewDir);
	}
	// 3. Прибавление света от фонарика
	if (hasSpotLight){
		result += CalcSpotLight(spotLight, normal, fragPosition, viewDir);
	}

	// 4. Прибавление собственного свечения
	vec3 emission = vec3(texture(material.emission, fTextureCoord));
	result += emission;

	fragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction); // Вектор направления света

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(normal, lightDir), 0.0);

	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, normal);
	// shininess - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Комбинируем результаты:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, fTextureCoord));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord)) * attenuation;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord)) * attenuation;
	vec3 specular = light.specular * spec * specularMap * attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord)) * intensity_angle * attenuation;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord)) * intensity_angle * attenuation;
	vec3 specular = light.specular * spec * specularMap * intensity_angle * attenuation;
	return (ambient + diffuse + specular);
}