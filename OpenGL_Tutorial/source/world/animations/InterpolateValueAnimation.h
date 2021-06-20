#pragma once
#include <glm/glm.hpp>
#include "LinearAnimation.h"

template<class T, class Value>
class InterpolateValueAnimation : public LinearAnimation<T>
{
	private:
	Value startValue;
	Value endValue;

	void (T::* setValueFunction)(Value);

	void setNewValue(const float& currentTime)
	{
		const Value currentValue = getCurrentState(currentTime, startValue, endValue);
		(this->object->*setValueFunction)(currentValue);
	}

	Value getCurrentState(const float& currentTime, const Value& a, const Value& b) const
	{
		const Value interpolatedValue = (Value)((b - a) * this->getInterpolationValue(currentTime)) + a;
		return interpolatedValue;
	}

	virtual void actCore(const float& currentTime) override
	{
		setNewValue(currentTime);
	}
public:
	InterpolateValueAnimation(T *object, float length, Value startValue, Value endValue, void (T::* setValueFunction)(Value))
		: LinearAnimation<T>(object, length), startValue(startValue), endValue(endValue), setValueFunction(setValueFunction)
	{
	}

	void endImmediately() override
	{
		(this->object->*setValueFunction)(endValue);
	}
};

template<class T>
class FloatAnimation : public InterpolateValueAnimation<T, float>
{
public:
	FloatAnimation(T *object, float length, float startValue, float endValue, void (T::* setValueFunction)(float))
		:InterpolateValueAnimation<T, float>(object, length, startValue, endValue, setValueFunction)
	{
	}
};

template<class T>
class UnsignedAnimation : public InterpolateValueAnimation<T, unsigned>
{
public:
	UnsignedAnimation(T *object, float length, unsigned startValue, unsigned endValue, void (T::* setValueFunction)(unsigned))
		:InterpolateValueAnimation<T, unsigned>(object, length, startValue, endValue, setValueFunction)
	{
	}
};

template<class T>
class Vec3Animation : public InterpolateValueAnimation<T, const glm::vec3&>
{
public:
	Vec3Animation(T *object, float length, const glm::vec3& startValue, const glm::vec3& endValue, void (T::* setValueFunction)(const glm::vec3&))
		:InterpolateValueAnimation<T, const glm::vec3&>(object, length, startValue, endValue, setValueFunction)
	{
	}
};
