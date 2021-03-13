#include "Camera.h"

using namespace glm;

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) 
	: position(pos), worldUp(up), front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVITY), FOV(DEFAULT_FOV),
	  yaw(yaw), pitch(pitch)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: position(vec3(posX, posY, posZ)), worldUp(vec3(upX, upY, upZ)), movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVITY), FOV(DEFAULT_FOV),
	  yaw(yaw), pitch(pitch)
{
	updateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	return lookAt(position, position + front, up);
}

void Camera::handleKeyboard(MovementDirection direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case STRAFE_LEFT:
		position -= right * velocity;
		break;
	case STRAFE_RIGHT:
		position += right * velocity;
		break;
	case UP:
		position += up * velocity;
		break;
	case DOWN:
		position -= up * velocity;
		break;
	case ROLL_LEFT:
		break;
	case ROLL_RIGHT:
		break;
	}
}

void Camera::handleMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::handleMouseScroll(float yOffset)
{
	FOV -= yOffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	else if (FOV > 90.0f)
		FOV = 90.0f;
}

void Camera::updateCameraVectors()
{
	// ��������� ������ ����������� ������:
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
