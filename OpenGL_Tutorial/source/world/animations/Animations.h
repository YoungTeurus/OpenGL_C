#pragma once
#include "FloatAnimation.h"

class Animations
{
	static LinearAnimation* rotateToAngle(PositionedWorldObject *object, const float& length, const float& endValue)
	{
		float startAngle = object->getRotationAngleDegrees();

		void (PositionedWorldObject::* setFunction)(const float&) = PositionedWorldObject::setRotationAngleDegrees;
		
		return new FloatAnimation(
			object,
			length,
			startAngle,
			endValue,
			setFunction
		);
	}
};
