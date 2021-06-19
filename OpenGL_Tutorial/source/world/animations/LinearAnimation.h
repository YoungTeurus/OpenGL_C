#pragma once
#include <algorithm>
#include <glm/common.hpp>

#include "Animation.h"
#include "../PositionedWorldObject.h"

class LinearAnimation : public Animation
{
protected:
	LinearAnimation(PositionedWorldObject *object, const float& length)
		:Animation(object, length)
	{
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
