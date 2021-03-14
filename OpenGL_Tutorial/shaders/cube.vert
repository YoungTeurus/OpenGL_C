#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 fColor;
out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fragPosition;  // Положение точки в мировых координатах

// Передаём отдельно матрицы трансформации и уже посчитанную матрицу
// С одной стороны - плохо, ибо лишние данные, с другой - быстро умножаем на точку?
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transformation;  // Перемножение трёх матриц выше в порядке: P * V * M
uniform mat3 normal;  // Матрица нормалей - используется для поворота/скейлинга

void main()
{
	gl_Position = transformation * vec4(aPos, 1.0f);
	fragPosition = vec3(model * vec4(aPos, 1.0f));
	fNormal = normal * aNormal;

	fColor = aColor;
	fTexCoord = aTexCoord;
}