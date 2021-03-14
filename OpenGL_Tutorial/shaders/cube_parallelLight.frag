#version 330 core

struct Material {
	sampler2D	diffuse;	// Карта освещения (текстура)
	sampler2D	specular;	// Карта для спекулярного света (текстура)

	sampler2D	emission;	// Карта свечения

	float		shininess;	// Глянцевость поверхности
};

struct Light {
	vec3 direction;		// Направление параллельного света

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света
};

in vec3 fNormal;
in vec3 fragPosition;
in vec2 fTextureCoord;

out vec4 fragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;  // Положение камеры в мировых координатах

void main(){
	// Мировое освещение:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTextureCoord));

	// Диффузионный свет:
	vec3 norm = normalize(fNormal);  // Нормаль к поверхности
	vec3 lightDir = normalize(-light.direction); // Вектор направления света

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
	vec3 specularMap = vec3(texture(material.specular, fTextureCoord));
	vec3 specular = light.specular * spec * specularMap;

	// Свечение объекта
	vec3 emission = vec3(texture(material.emission, fTextureCoord)) * (vec3(1.0) * int(specularMap.r == 0));

	vec3 result = ambient + diffuse + specular + emission;

	fragColor = vec4(result, 1.0);
}