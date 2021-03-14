#version 330 core

in vec3 fNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;  // Положение камеры в мировых координатах

void main(){
	float ambientStrength = 0.1;
	float specularStrength = 0.5;

	// Мировое освещение:
	vec3 ambient = ambientStrength * lightColor;

	// Диффузионный свет:
	vec3 norm = normalize(fNormal);  // Нормаль к поверхности
	vec3 lightDir = normalize(lightPos - fragPosition); // Вектор направления света

	// Скалярное произведение, даёт косинус угла [0,1], так как оба вектора нормализованы
	// diff = 1, когда угол = 0 (свет падает перпендикулярно)
	float diff = max( dot(norm, lightDir), 0.0);

	vec3 diffuse = diff * lightColor;

	// Specular свет:
	vec3 viewDir = normalize(viewPos - fragPosition);
	// Отражаем падающий свет отнсоительно вектора нормали
	vec3 reflectDir = reflect(-lightDir, norm);

	// 32 - степень глянцевости поверхности. Чем больше, тем более чётко выделено светлое пятно
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(result, 1.0);
}