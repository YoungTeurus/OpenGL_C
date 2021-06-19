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
			if (startAngle <= endValue)
			{
				startAngle = startAngle + 360.0f;
			} else
			{
				endAngle = endAngle + 360.0f;
			}
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
