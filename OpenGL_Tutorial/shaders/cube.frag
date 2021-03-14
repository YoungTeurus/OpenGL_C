#version 330 core

struct Material {
	vec3 ambient;		// Цвет объекта в мировом освещении
	vec3 diffuse;		// Цвет объекта в диффузном освещении
	vec3 specular;		// Цвет объекта в спекулярном освещении
	float shininess;	// Глянцевость поверхности
};

struct Light {
	vec3 position;		// Месторасположение источника света

	vec3 ambient;		// Цвет (сила?) мирового света
	vec3 diffuse;		// Цвет (сила?) диффузного света
	vec3 specular;		// Цвет (сила?) спекулярного света
};

in vec3 fNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;  // Положение камеры в мировых координатах

void main(){
	// Мировое освещение:
	vec3 ambient = material.ambient * light.ambient;

	// Диффузионный свет:
	vec3 norm = normalize(fNormal);  // Нормаль к поверхности
	vec3 lightDir = normalize(light.position - fragPosition); // Вектор направления света

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	// Specular свет:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, norm);

	// 32 - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = (material.specular * spec) * light.specular;

	vec3 result = ambient + diffuse + specular;

	fragColor = vec4(result, 1.0);
}