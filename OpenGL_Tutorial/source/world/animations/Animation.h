#pragma once
#include "../interfaces/PositionedObject.h"

class Animation
{
protected:
	bool hasStarted = false;
	bool hasEnded = false;

	PositionedObject *object;
	float length;  // Длительность анимации
	float startTime;  // Время начала анимации

	Animation(PositionedObject *object, const float& length)
		:object(object), length(length), startTime(0.0f)
	{
	}

	void checkAndSetIfEnded(const float& currentTime)
	{
		if (currentTime - startTime > length)
		{
			hasEnded = true;
		}
	}
public:
	virtual void act(const float& currentTime) = 0;
	virtual void endImmediately() = 0;

	virtual ~Animation() = default;

	bool isHasEnded() const
	{
		return hasEnded;
	}
};
