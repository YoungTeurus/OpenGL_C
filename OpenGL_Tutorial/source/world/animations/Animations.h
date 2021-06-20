#pragma once
#include "FloatAnimation.h"
#include "PackedAnimation.h"
#include "../implementations/Tank.h"

class Animations
{
public:
	static Animation<PositionedObject>* rotateToAngle(PositionedObject *object, const float& length, const float& endValue)
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

		void (PositionedObject::* setFunction)(const float&) = &PositionedObject::setRotationAngleDegrees;
		
		return new FloatAnimation<PositionedObject>(
			object,
			length,
			startAngle,
			endAngle,
			setFunction
		);
	}

	static Animation<Tank>* blowTank(Tank* tank, const float& length, const float& explosionForce)
	{
		PackedAnimation<Tank> *animation = new PackedAnimation<Tank>(tank, length);

		void (Tank::* setExplosionMagnitude)(const float&) = &Tank::setExplosionMagnitude;
		void (Tank::* setTimeSinceExplosion)(const float&) = &Tank::setTimeSinceExplosion;

		animation->addAnimation(
			new FloatAnimation<Tank>(
				tank,
				length,
				0.0f,
				explosionForce,
				setExplosionMagnitude
			)
		);
	}
};
