#pragma once
#include "Animation.h"
#include "../PositionedWorldObject.h"
#include "../UpdatableObject.h"

class AnimatedObject : public PositionedWorldObject, public UpdatableObject
{
private:
	Animation* currentAnimation = nullptr;
public:
	AnimatedObject(const ModelTransformations& transformations)
		:PositionedWorldObject(transformations)
	{
	}
	
	void update(const float& currentTime) override
	{
		UpdatableObject::update(currentTime);
		
		currentAnimation->act(currentTime);
		
		if (currentAnimation->isHasEnded())
		{
			delete currentAnimation;
			currentAnimation = nullptr;
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
