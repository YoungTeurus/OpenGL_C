#version 330 core

#define MAX_LIGHTS 8

struct Material {
	sampler2D	diffuse;	// Карта освещения (текстура)
	sampler2D	specular;	// Карта для спекулярного света (текстура)

	sampler2D	emission;	// Карта свечения

	float		shininess;	// Глянцевость поверхности
};

struct Light{
	int type;

	vec3 position;

	// Для Direction и Spot источников:
	vec3 direction;

	// Для Spot источника:
	float cutOff;

	// Цвет источника:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Для затухающих источников:
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

uniform vec3 viewPos;  // Положение камеры в мировых координатах

// Объявление функций:

// Вовзвращает число в диапазоне [0;1], отражающее относительную яркость света источника света.
// Уменьшается с увеличением расстояния до источника света.
float getAttenuation(int i);
// Вовзвращает подсчитанный цвет (текстуры) для диффузного и спекулярного освещения для i-ого источника света.
// Свет находится в направлении lightDirection от объекта.
vec3 getDiffuseAndSpecularColor(int i, vec3 lightDirection);


void main(){
	vec3 currentLightColorResult;  // Временный вектор для хранения подсчитанного цвета для текущего источника света
	for(int i = 0; i < lightsCount; i++){
		if (lights[i].type == 1){ // Направленный свет:
			// Вектор направления К источнику света (подразумевается, что передан нормализованный вектор):
			vec3 lightDirection = -lights[i].direction;

			// Цвет неосвещённого участка:
			vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

			// Цвет собственно от освещения:
			vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, lightDirection);

			currentLightColorResult = ambientColor + diffuseAndSpecularColor;
		} else {
			// Нормализованный вектор направления ОТ точки падения К источнику света:
			vec3 fromCollisionPointToLightDirection = normalize(lights[i].position - fFragPosition);
			if( lights[i].type == 2) {  // Точечный источник света:
				// Степень затухания света:
				float attenuation = getAttenuation(i);

				// Цвет неосвещённого участка:
				vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

				// Цвет собственно от освещения:
				vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, fromCollisionPointToLightDirection);

				currentLightColorResult = (ambientColor + diffuseAndSpecularColor) * attenuation;
			} else if (lights[i].type == 3) {  // Конусовидный источник света:
				// Угол между нормализованными вектором направления К источнику света и вектором направления ОТ точки падения К источнику света:
				float angle = acos(dot(fromCollisionPointToLightDirection, normalize(-lights[i].direction)));

				// Если видна хотя бы часть конуса:
				if (angle <= lights[i].cutOff * 2.0f){
					// Стандартный коэффициент яркости освещения:
					float koef = 1.0f;
					
					if (angle >= lights[i].cutOff){
						// Уменьшаем koef в диапазоне [0;1] в зависимости от угла:
						koef = (lights[i].cutOff*2.0f - angle) / lights[i].cutOff;
					}

					// Степень затухания света:
					float attenuation = getAttenuation(i);

					// Цвет неосвещённого участка:
					vec3 ambientColor = lights[i].ambient * texture(texture_diffuse1, fTextureCoord).rgb;

					// Цвет собственно от освещения:
					vec3 diffuseAndSpecularColor = getDiffuseAndSpecularColor(i, fromCollisionPointToLightDirection) * koef;

					currentLightColorResult = (ambientColor + diffuseAndSpecularColor) * attenuation;
				} else {  // angle > lights[i].cutOff * 2.0f
					// Цвет неосвещённого участка:
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
	// Получаем значение нормали в диапазоне [0;1] :
	vec3 normal = texture(texture_normal1, fTextureCoord).rgb;
	// Получаем нормаль в диапазоне [-1;1] :
	normal = normal * 2.0f - 1.0f;
	// Получаем нормаль в диапазоне [-1;1] :
	normal = normalize(normal);
	// ?!
	normal = normalize(TBN * normal);


	// Скалярное произведение нормализованных векторов нормали и направления к источнику света. Может быть в диапазоне [-1;1] :
	float angleDot = dot(normal, lightDirection);
	// Яркость диффузного освещения: [0;1]
	float diffuseKoeff = max(angleDot, 0.0);
	vec3 diffuseColor = lights[i].diffuse * diffuseKoeff * texture(texture_diffuse1, fTextureCoord).rgb;

	// Направление отражённого света:
	vec3 reflectDirection = reflect(lightDirection, normal);
	// Направление ОТ положения камеры К точке падения:
	vec3 viewDirection = normalize(fragPosition - viewPos);
	// Скалярное произведение векторов направлений взгляда и отражённого луча [-1;1]:
	float viewAndReflectDirectionsDotProduct = dot(viewDirection, reflectDirection);

	// viewAndReflectDirectionsDotProduct отрицательный?! (отражённый вектор противолежит взгляду камеры)

	// Яркрость спекулярного света:
	float specularKoeff = pow(max(viewAndReflectDirectionsDotProduct, 0.0f), shininess);
	vec3 specularColor = lights[i].specular * specularKoeff * texture(texture_specular1, fTextureCoord).rgb;

	return diffuseColor + specularColor;
}