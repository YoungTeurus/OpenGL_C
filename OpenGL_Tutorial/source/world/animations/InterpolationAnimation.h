#pragma once
#include "Animation.h"

template<class T>
class InterpolationAnimation : public Animation<T>
{
protected:
	virtual float getInterpolationValue(const float& currentTime) const = 0;
public:
	InterpolationAnimation(T* object, const float& length)
		: Animation<T>(object, length)
	{
	}

	virtual ~InterpolationAnimation() = default;
};
