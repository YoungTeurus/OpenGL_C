#pragma once
#include <vector>

#include "Animation.h"
#include "../interfaces/PositionedObject.h"


class PackedAnimation : public Animation
{
private:
	std::vector<Animation*> animations;
public:
	PackedAnimation(PositionedObject *object, const float& length)
		:Animation(object, length)
	{
	}

	PackedAnimation* addAnimation(Animation* animation)
	{
		animations.push_back(animation);
		return this;
	}

	void act(const float& currentTime) override
	{
		for (auto &&animation : animations)
		{
			animation->act(currentTime);
		}
	}
};
