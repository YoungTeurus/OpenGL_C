#pragma once
#include "LinearAnimation.h"

class FloatAnimation : public LinearAnimation
{
private:
	float startValue;
	float endValue;

	void (PositionedWorldObject::* setValueFunction)(const float&);
public:
	FloatAnimation(PositionedWorldObject *object, float length, float startValue, float endValue, void (PositionedWorldObject::* setValueFunction)(const float&))
		: LinearAnimation(object, length), startValue(startValue), endValue(endValue), setValueFunction(setValueFunction)
	{
	}

	virtual void act(const float& currentTime) override
	{
		if (!hasStarted)
		{
			this->startTime = currentTime;
			hasStarted = true;
		}
		if (hasEnded)
		{
			return;
		}

		const float currentValue = getCurrentState(currentTime, startValue, endValue);
		(getObject()->*setValueFunction)(currentValue);
		
		checkAndSetIfEnded(currentTime);
	}

	float getCurrentState(const float& currentTime, const float& a, const float& b) const
	{
		const float interpolatedValue = (b - a) * getInterpolationValue(currentTime) + a;
		return interpolatedValue;
	}

	float getStartValue() const
	{
		return startValue;
	}

	float getEndValue() const
	{
		return endValue;
	}

	void endImmediately() override
	{
		(getObject()->*setValueFunction)(endValue);
	}
};

//class Vector3Animation : LinearAnimation
//{
//private:
//	glm::vec3 startValue;
//	glm::vec3 endValue;
//
//	void(*setValueFunction)(glm::vec3);
//public:
//	Vector3Animation(float length, glm::vec3(*getStartValueFunction)(void), const glm::vec3& endValue, void(*setValueFunction)(glm::vec3))
//		: LinearAnimation(length), startValue(getStartValueFunction()), endValue(endValue), setValueFunction(setValueFunction)
//	{
//	}
//
//	void act(const float& currentTime) override
//	{
//		if (!hasStarted)
//		{
//			startTime = currentTime;
//		}
//
//		glm::vec3 currentValue = getCurrentState(currentTime, startValue, endValue);
//		setValueFunction(currentValue);
//	}
//
//	glm::vec3 getCurrentState(const float& currentTime, const glm::vec3& a, const glm::vec3& b) const
//	{
//		const glm::vec3 interpolatedValue = (b - a) * getInterpolationValue(currentTime) + a;
//		return interpolatedValue;
//	}
//};

//int main()
//{
//	Tank tank;
//	if (нажали_вправо && танк_не_смотрит_вправо)
//	{
//		tank.setAnimation(Animations::rotateToAngle(this, угол_направления_вправо, время));
//	}
//}
//
//Tank::update{
//	if (currentAnimation != nullptr)
//	{
//		currentAnimation.act(currentTime, this);
//	}
//}
//
//Animations::rotateToAngle(угол_направления, время)
//{
//	return new FloatAnimation(время, PositionedWorldObject::getRotateAngle, угол_направления, PositionedWorldObject::setRotateAngle);
//}
