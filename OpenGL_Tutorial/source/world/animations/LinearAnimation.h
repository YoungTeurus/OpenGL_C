#pragma once
#include <algorithm>

#include "InterpolationAnimation.h"

template<class T>
class LinearAnimation : public InterpolationAnimation<T>
{
protected:
	LinearAnimation(T *object, const float& length)
		:InterpolationAnimation<T>(object, length)
	{
	}

	float getInterpolationValue(const float& deltaTime) override
	{
		const float interpolationValue = std::min(1.0f, std::max(0.0f, this->timeSinceStart/this->length));
		return interpolationValue;
	}
};
