#pragma once
#include "FloatAnimation.h"

class Animations
{
public:
	static Animation* rotateToAngle(PositionedWorldObject *object, const float& length, const float& endValue)
	{
		float startAngle = object->getRotationAngleDegrees();
		float endAngle = endValue;

		if (abs(endValue - startAngle) > 180.0f)
		{
			endAngle = 360.0f - endValue;
		}

		void (PositionedWorldObject::* setFunction)(const float&) = &PositionedWorldObject::setRotationAngleDegrees;
		
		return new FloatAnimation(
			object,
			length,
			startAngle,
			endAngle,
			setFunction
		);
	}
};
