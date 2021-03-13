#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MovementDirection {
	FORWARD,
	BACKWARD,
	STRAFE_LEFT,
	STRAFE_RIGHT,
	UP,
	DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
};

// Значения по умолчанию:
const float DEFAULT_YAW			= -90.0f; // Стандарное рыскание: камера направлена по оси -Z
const float DEFAULT_PITCH		=   0.0f;
const float DEFAULT_SPEED		=   2.5f;
const float DEFAULT_SENSITIVITY =   0.1f;
const float DEFAULT_FOV			=  45.0f;

class Camera
{
public:
	glm::vec3 position;		// Позиция камеры в мире
	glm::vec3 front;		// Направление взгляда камеры ("вперёд")
	glm::vec3 up;			// Направление вектора "вверх" камеры
	glm::vec3 right;		// Направление вектора "вправо" камеры
	glm::vec3 worldUp;		// Направление вектора "вверх" мира

	float yaw;				// Рыскание камеры: вращение вокруг оси Y (по часовой стрелке) - влево-вправо
	float pitch;			// Тангаж камеры: вращение вокруг оси X (по часовой стрелке) - вниз-вверх

	float movementSpeed;	// Скорость движения камеры
	float mouseSensitivity;	// Чувствительность мыши
	float FOV;				// Угол зрения камеры

	// Конструктор камеры с векторными параметрами
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);

	// Конструктор камеры со скалярными параметрами
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Деструктор камеры
	~Camera();

	// Получение видовой матрицы Look At
	glm::mat4 getViewMatrix();

	// Перемещение камеры в указанном направлении в течение deltaTime
	void handleKeyboard(MovementDirection direction, float deltaTime);

	// Поворот камеры при перемещении мыши на xOffset и yOffset. При необходимости ограничивает тангаж
	void handleMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	// Изменение FOV камеры при скролле колёсика
	void handleMouseScroll(float yOffset);

private:
	// Рассчитывает векторы
	void updateCameraVectors();
};

