#pragma once
#include <algorithm>
#include <glm/common.hpp>

#include "Animation.h"
#include "../PositionedWorldObject.h"

class LinearAnimation : Animation
{
protected:
	bool hasStarted = false;
	bool hasEnded = false;
	
	float length;  // Длительность анимации
	float startTime;  // Время начала анимации

	LinearAnimation(PositionedWorldObject *object, const float& length)
		:Animation(object, length)
	{
	}

	bool checkAndSetIfEnded(const float& currentTime)
	{
		if (currentTime - startTime > length)
		{
			hasEnded = true;
		}
	}

	float getInterpolationValue(const float& currentTime) const override
	{
		const float timeSinceStart = currentTime - startTime;
		const float interpolationValue = std::min(1.0f, glm::max(0.0f, timeSinceStart/length));
		return interpolationValue;
	}

	PositionedWorldObject* getObject() const
	{
		return object;
	}
};
