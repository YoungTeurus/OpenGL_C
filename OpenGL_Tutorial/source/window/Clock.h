#pragma once
#include <glad\glad.h>
#include <GLFW/glfw3.h>

class Clock
{
private:
	float previousTimestamp = 0.0f;
	float currentTimestamp = 0.0f;
	float deltaTime = 0.0f;
public:
	void updateTime()
	{
		previousTimestamp = currentTimestamp;
		currentTimestamp = (float)glfwGetTime();
		deltaTime = currentTimestamp - previousTimestamp;
	}

	float getCurrentTimestamp() const
	{
		return currentTimestamp;
	}

	float getDeltaTime() const
	{
		return deltaTime;
	}
};
