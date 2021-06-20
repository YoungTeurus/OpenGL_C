#pragma once
#include "Animation.h"

class InterpolationAnimation : public Animation
{
protected:
	virtual float getInterpolationValue(const float& currentTime) const = 0;
public:
	InterpolationAnimation(PositionedObject* object, const float& length)
		: Animation(object, length)
	{
	}

	virtual ~InterpolationAnimation() = default;
};
