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
private:
	void calculateAspectRatio()
	{
		this->aspectRatio = (float)this->viewSize.x / this->viewSize.y;
	}
public:
	glm::vec3 position;		// ������� ������ � ����
	glm::vec3 front;		// ����������� ������� ������ ("�����")
	glm::vec3 up;			// ����������� ������� "�����" ������
	glm::vec3 right;		// ����������� ������� "������" ������
	glm::vec3 worldUp;		// ����������� ������� "�����" ����

	glm::ivec2 viewSize;	// ������ ������� ������� ������.

	float yaw;				// �������� ������: �������� ������ ��� Y (�� ������� �������) - �����-������
	float pitch;			// ������ ������: �������� ������ ��� X (�� ������� �������) - ����-�����

	float movementSpeed;	// �������� �������� ������
	float mouseSensitivity;	// ���������������� ����
	float FOV;				// ���� ������ ������

	float zNear;
	float zFar;
	float aspectRatio;

	// ����������� ������ � ���������� �����������
	Camera(const glm::ivec2& viewSize, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH, float zNear = 1.0f, float zFar = 100.0f);

	// ����������� ������ �� ���������� �����������
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, const glm::ivec2& viewSize, float zNear = 1.0f, float zFar = 100.0f);

	// ���������� ������
	~Camera();

	void setViewSize(const glm::ivec2& viewSize)
	{
		this->viewSize = viewSize;
		calculateAspectRatio();
	}

	void setPosition(const glm::vec3& position)
	{
		this->position = position;
		updateCameraVectors();
	}

	void setYawAndPitch(const float& yaw, const float& pitch)
	{
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// ��������� ������� ������� Look At
	glm::mat4 getViewMatrix();
	
	glm::mat4 getProjectionMatrix();

	glm::mat4 getOrthoProjection()
	{
		return glm::ortho(0.0f, (float)viewSize.x, 0.0f, (float)viewSize.y, -1.0f, 1.0f);
	}

	// ����������� ������ � ��������� ����������� � ������� deltaTime
	void handleKeyboard(MovementDirection direction, float deltaTime);

	// ������� ������ ��� ����������� ���� �� xOffset � yOffset. ��� ������������� ������������ ������
	void handleMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	// ��������� FOV ������ ��� ������� �������
	void handleMouseScroll(float yOffset);

	glm::vec3 screenCoordToWorldCoords(const glm::vec2& screenCoords)
	{
		const glm::vec4 clipSpaceCoords(2.0f * screenCoords.x / viewSize.x - 1.0f, -2.0f * (float)screenCoords.y/viewSize.y + 1.0f, -1.0f, 1.0f);
		const glm::vec4 tmp = glm::inverse(getProjectionMatrix()) * clipSpaceCoords;
		const glm::vec4 viewSpaceCoords(tmp.x, tmp.y, -1.0f, 0.0f);

		const glm::vec3 worldSpaceDirection = glm::normalize(glm::inverse(getViewMatrix()) * viewSpaceCoords);

		// Ax + By + Cz + R = 0
		// N = (A, B, C)
		// R - ���������� �� �����. �����
		// P * N + d = 0
		// d = - P0 * N;
		// O + Dir * t
		// (O + Dir * t) * N - P0 * N = 0
		// t = (P0 * N - O * N) / (Dir * N)
		// result = O + Dir * t

		glm::vec3 N(0.0f, 1.0f, 0.0f); // ������� � �����������
		glm::vec3 O = position; // ������� ������

		float t = (- glm::dot(O, N)) / glm::dot(worldSpaceDirection, N);
		glm::vec3 result = O + worldSpaceDirection * t;

		return result;
	}

private:
	// ������������ �������
	void updateCameraVectors();
};

