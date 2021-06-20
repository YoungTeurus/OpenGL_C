#pragma once
#include "Animation.h"
#include "../interfaces/UpdatableObject.h"

class AnimatedObject : public PositionedObject, public UpdatableObject
{
private:
	Animation<PositionedObject>* currentAnimation = nullptr;
public:
	AnimatedObject(const ModelTransformations& transformations)
		:PositionedObject(transformations)
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
	
	void addAnimation(Animation<PositionedObject>* animation)
	{
		if (currentAnimation != nullptr)
		{
			currentAnimation->endImmediately();
			delete currentAnimation;
		}
		currentAnimation = animation;
	}
};
