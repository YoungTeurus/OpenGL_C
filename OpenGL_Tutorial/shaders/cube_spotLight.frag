#version 330 core

struct Material {
	sampler2D	diffuse;	// Карта освещения (текстура)
	sampler2D	specular;	// Карта для спекулярного света (текстура)

	sampler2D	emission;	// Карта свечения

	float		shininess;	// Глянцевость поверхности
};

struct Light {
	vec3 position;
	vec3 direction;		// Направление света
	float cutOffCosin;	// Косинус угла распространения света со 100% интенсивностью
	float outerCutOffCosin;	// Косинус угла распространения света, за которым интенсивность = 0%

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света

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

uniform vec3 viewPos;  // Положение камеры в мировых координатах

void main(){
	vec3 vertToLightVec = light.position - fragPosition;
	float vertToLightDistance = length(vertToLightVec);

	float intencity = 1.0 / (light.constant + light.linear * vertToLightDistance + light.quadratic * vertToLightDistance * vertToLightDistance);

	// Мировое освещение:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));

	// Направление от вертекса к свету
	vec3 lightDir = normalize(vertToLightVec); // Вектор направления света

	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));
	// Свечение объекта
	vec3 emission = vec3(texture(material.emission, fTextureCoord)) * (vec3(1.0) * int(specularMap.r == 0));

	// Эффект spotlight:
	// Находим косинус угла между направлением фонаря и направлением от объекта к свету
	float cosinBetweenLightAndVertex = dot(lightDir, normalize(-light.direction));
	// Используем форумулу: I = (0 - y) / e (https://learnopengl.com/Lighting/Light-casters) , чтобы сделать плавную границу света
	float difference = light.cutOffCosin - light.outerCutOffCosin;
	float intensity = clamp((cosinBetweenLightAndVertex - light.outerCutOffCosin)/difference, 0.0, 1.0);
	
	// Диффузионный свет:
	vec3 norm = normalize(fNormal);  // Нормаль к поверхности
	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTextureCoord));

	// Specular свет:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, norm);

	// shininess - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * specularMap;

	diffuse *= intensity * intencity;
	specular *= intensity * intencity;

	vec3 result = ambient + diffuse + specular + emission;

	fragColor = vec4(result, 1.0);
}