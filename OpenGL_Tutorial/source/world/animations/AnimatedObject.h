#pragma once
#include "Animation.h"
#include "../interfaces/UpdatableObject.h"

template<class T>
class AnimatedObject : public UpdatableObject
{
private:
	Animation<T>* currentAnimation = nullptr;
public:
	AnimatedObject() = default;
	
	virtual void update(const float& currentTime) override
	{
		UpdatableObject::update(currentTime);

		if (currentAnimation)
		{
			currentAnimation->act(currentTime);
		
			if (currentAnimation->isHasEnded())
			{
				delete currentAnimation;
				currentAnimation = nullptr;
			}
		}
	}
	
	void addAnimation(Animation<T>* animation)
	{
		if (currentAnimation != nullptr)
		{
			currentAnimation->endImmediately();
			delete currentAnimation;
		}
		currentAnimation = animation;
	}
};

class ActualAnimatedObject : public AnimatedObject<ActualAnimatedObject>
{
	
};



template<class T>
class Anim
{
private:
	T obj;
public:
	void use()
	{
	}
};
