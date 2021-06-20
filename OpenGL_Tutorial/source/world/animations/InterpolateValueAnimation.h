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

	virtual Value getCurrentState(const float& currentTime, const Value& a, const Value& b) const
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
private:
	virtual unsigned getCurrentState(const float& currentTime, const unsigned& a, const unsigned& b) const override
	{
		const unsigned _b = std::max(a, b), _a = std::min(a, b);
		const unsigned delta = _b - _a;
		const unsigned interpolatedValue = (unsigned)(delta * this->getInterpolationValue(currentTime)) + _a;
		return interpolatedValue;
	}
public:
	UnsignedAnimation(T *object, float length, unsigned startValue, unsigned endValue, void (T::* setValueFunction)(unsigned))
		:InterpolateValueAnimation<T, unsigned>(object, length, startValue, endValue, setValueFunction)
	{
	}
};

template<class T>
class Vec3Animation : public InterpolateValueAnimation<T, glm::vec3>
{
private:
	virtual glm::vec3 getCurrentState(const float& currentTime, const glm::vec3& a, const glm::vec3& b) const override
	{
		const float interValue = this->getInterpolationValue(currentTime);
		const glm::vec3 delta = b - a;
		const glm::vec3 deltaXInterValue = delta * interValue;
		const glm::vec3 interpolatedValue = deltaXInterValue + a;
		return interpolatedValue;
	}
public:
	Vec3Animation(T *object, float length, glm::vec3 startValue, glm::vec3 endValue, void (T::* setValueFunction)(glm::vec3))
		:InterpolateValueAnimation<T, glm::vec3>(object, length, startValue, endValue, setValueFunction)
	{
	}
};
