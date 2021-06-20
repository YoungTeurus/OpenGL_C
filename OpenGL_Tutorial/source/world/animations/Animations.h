#pragma once
#include "FloatAnimation.h"
#include "PackedAnimation.h"
#include "../implementations/Tank.h"
#include "DelayedAction.h"

class Animations
{
public:
	template<class T>
	static Animation<T>* rotateToAngle(T *object, const float& length, const float& endValue)
	{
		static_cast<PositionedObject*>((T*)0);
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

		auto *animation = new FloatAnimation<T>(
			object,
			length,
			startAngle,
			endAngle,
			setFunction
		);
		
		return animation;
	}

	static Animation<Tank>* blowTank(Tank* tank, const float& length, const float& explosionForce)
	{
		auto *animation = new PackedAnimation<Tank>(tank, length);

		void (Tank::* setExplosionMagnitude)(const float&) = &Tank::setExplosionMagnitude;
		void (Tank::* setTimeSinceExplosion)(const float&) = &Tank::setTimeSinceExplosion;
		void (Tank::* setVisible)(bool) = &Tank::setVisible;

		animation->addAnimation(
			new FloatAnimation<Tank>(
				tank,
				length,
				0.0f,
				explosionForce,
				setExplosionMagnitude
			)
		);

		animation->addAnimation(
			new FloatAnimation<Tank>(
				tank,
				length,
				0.0f,
				length,
				setTimeSinceExplosion
			)
		);

		animation->addAnimation(
			new DelayedSingleParameterAction<Tank, bool>(
				tank,
				length,
				setVisible,
				false
			)
		);

		return animation;
	}
};
