#pragma once
#include "LinearAnimation.h"

template<class T>
class FloatAnimation : public LinearAnimation<T>
{
private:
	float startValue;
	float endValue;

	void (T::* setValueFunction)(const float&);
public:
	FloatAnimation(T *object, float length, float startValue, float endValue, void (T::* setValueFunction)(const float&))
		: LinearAnimation<T>(object, length), startValue(startValue), endValue(endValue), setValueFunction(setValueFunction)
	{
	}

	virtual void act(const float& currentTime) override
	{
		if (!this->hasStarted)
		{
			this->startTime = currentTime;
			this->hasStarted = true;
		}
		if (this->hasEnded)
		{
			return;
		}

		const float currentValue = getCurrentState(currentTime, startValue, endValue);
		(this->object->*setValueFunction)(currentValue);
		
		this->checkAndSetIfEnded(currentTime);
	}

	float getCurrentState(const float& currentTime, const float& a, const float& b) const
	{
		const float interpolatedValue = (b - a) * this->getInterpolationValue(currentTime) + a;
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
		(this->object->*setValueFunction)(endValue);
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
//	return new FloatAnimation(время, PositionedObject::getRotateAngle, угол_направления, PositionedObject::setRotateAngle);
//}
