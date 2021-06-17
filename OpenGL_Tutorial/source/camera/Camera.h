#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class MovementDirection {
	FORWARD,
	BACKWARD,
	STRAFE_LEFT,
	STRAFE_RIGHT,
	UP,
	DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
};

// �������� �� ���������:
const float DEFAULT_YAW			= -90.0f; // ���������� ��������: ������ ���������� �� ��� -Z
const float DEFAULT_PITCH		=   0.0f;
const float DEFAULT_SPEED		=   2.5f;
const float DEFAULT_SENSITIVITY =   0.1f;
const float DEFAULT_FOV			=  45.0f;

class Camera
{
public:
	glm::vec3 position;		// ������� ������ � ����
	glm::vec3 front;		// ����������� ������� ������ ("�����")
	glm::vec3 up;			// ����������� ������� "�����" ������
	glm::vec3 right;		// ����������� ������� "������" ������
	glm::vec3 worldUp;		// ����������� ������� "�����" ����

	float yaw;				// �������� ������: �������� ������ ��� Y (�� ������� �������) - �����-������
	float pitch;			// ������ ������: �������� ������ ��� X (�� ������� �������) - ����-�����

	float movementSpeed;	// �������� �������� ������
	float mouseSensitivity;	// ���������������� ����
	float FOV;				// ���� ������ ������

	float zNear;
	float zFar;
	float aspectRatio;

	// ����������� ������ � ���������� �����������
	Camera(float aspectRatio, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH, float zNear = 1.0f, float zFar = 100.0f);

	// ����������� ������ �� ���������� �����������
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float aspectRatio, float zNear = 1.0f, float zFar = 100.0f);

	// ���������� ������
	~Camera();

	// ��������� ������� ������� Look At
	glm::mat4 getViewMatrix();

	
	glm::mat4 getProjectionMatrix();

	// ����������� ������ � ��������� ����������� � ������� deltaTime
	void handleKeyboard(MovementDirection direction, float deltaTime);

	// ������� ������ ��� ����������� ���� �� xOffset � yOffset. ��� ������������� ������������ ������
	void handleMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	// ��������� FOV ������ ��� ������� �������
	void handleMouseScroll(float yOffset);

	// ������������ �������
	void updateCameraVectors();
};

