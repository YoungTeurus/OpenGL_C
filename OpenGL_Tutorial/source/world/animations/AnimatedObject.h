#pragma once
#include "Animation.h"
#include "../interfaces/UpdatableObject.h"

class AnimatedObject : public PositionedWorldObject, public UpdatableObject
{
private:
	Animation* currentAnimation = nullptr;
public:
	AnimatedObject(const ModelTransformations& transformations)
		:PositionedWorldObject(transformations)
	{
	}
	
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
	
	void addAnimation(Animation* animation)
	{
		if (currentAnimation != nullptr)
		{
			currentAnimation->endImmediately();
			delete currentAnimation;
		}
		currentAnimation = animation;
	}
};
