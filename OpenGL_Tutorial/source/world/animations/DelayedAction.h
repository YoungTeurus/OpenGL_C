#pragma once
#include "Animation.h"
#include "../interfaces/PositionedObject.h"

template<class T>
class DelayedAction : public Animation<T>
{
protected:
	virtual void performAction(const float& currentTime) = 0;
public:
	DelayedAction(T *object, const float& delay)
		:Animation<T>(object, delay)
	{
	}

	virtual void actCore(const float& currentTime) override
	{
		if(currentTime - this->startTime > this->length)
		{
			performAction(currentTime);
		}
	}
};

template<class T>
class DelayedVoidAction : public DelayedAction<T>
{
private:
	void (T::* action)(void);
protected:
	virtual void performAction(const float& currentTime) override
	{
		(this->object->*action)();
	}
public:
	DelayedVoidAction(T *object, const float& delay, void (T::* action)(void))
		:DelayedAction<T>(object, delay), action(action)
	{
	}
};

template<class T, typename Arg>
class DelayedSingleParameterAction : public DelayedAction<T>
{
private:
	Arg actionParameter;
	void (T::* action)(Arg);
protected:
	virtual void performAction(const float& currentTime) override
	{
		(this->object->*action)(actionParameter);
	}
public:
	DelayedSingleParameterAction(T *object, const float& delay, void (T::* action)(Arg), Arg actionParameter)
		:DelayedAction<T>(object, delay), action(action), actionParameter(actionParameter)
	{
	}
};
