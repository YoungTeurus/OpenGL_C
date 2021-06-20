#pragma once
#include <algorithm>
#include <glm/common.hpp>

#include "InterpolationAnimation.h"

class LinearAnimation : public InterpolationAnimation
{
protected:
	LinearAnimation(PositionedObject *object, const float& length)
		:InterpolationAnimation(object, length)
	{
	}

	float getInterpolationValue(const float& currentTime) const override
	{
		const float timeSinceStart = currentTime - startTime;
		const float interpolationValue = std::min(1.0f, glm::max(0.0f, timeSinceStart/length));
		return interpolationValue;
	}

	PositionedObject* getObject() const
	{
		return object;
	}
};
