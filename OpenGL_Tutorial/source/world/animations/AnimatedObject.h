#pragma once
#include <queue>
#include "Animation.h"
#include "../interfaces/UpdatableObject.h"

template<class T>
class AnimatedObject : public UpdatableObject
{
private:
	Animation<T>* currentAnimation = nullptr;
	std::queue<Animation<T>*> animations;

	void setNextAnimation()
	{
		if(animations.empty())
		{
			this->currentAnimation = nullptr;
		} else {
			this->currentAnimation = animations.front();
			animations.pop();
		}
	}

	void endCurrentAnimation()
	{
		if (currentAnimation != nullptr)
		{
			currentAnimation->endImmediately();
			delete currentAnimation;
		}
	}

	void clearQueue(const bool& endAnimations = true)
	{
		while(!animations.empty())
		{
			if (endAnimations){
				animations.front()->endImmediately();
			}
			animations.pop();
		}
	}
	
public:
	AnimatedObject() = default;
	
	virtual void update(const float& deltaTime) override
	{
		UpdatableObject::update(deltaTime);

		if (currentAnimation)
		{
			currentAnimation->act(deltaTime);
		
			if (currentAnimation->isHasEnded())
			{
				delete currentAnimation;
				setNextAnimation();
			}
		} else
		{
			setNextAnimation();
		}
	}
	
	void addAnimation(Animation<T>* animation)
	{
		animations.push(animation);
	}

	void setAnimation(Animation<T>* animation, const bool& doEndCurrentAnimation = true, const bool& endOtherAnimations = true)
	{
		if(doEndCurrentAnimation){
			endCurrentAnimation();
		}
		clearQueue(endOtherAnimations);
		currentAnimation = animation;
	}
};
