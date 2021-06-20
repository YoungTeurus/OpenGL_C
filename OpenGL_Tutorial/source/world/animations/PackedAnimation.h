#pragma once
#include <vector>

#include "Animation.h"


class PackedAnimation final : public Animation
{
private:
	std::vector<Animation*> animations;
public:
	PackedAnimation(PositionedObject *object, const float& length)
		:Animation(object, length)
	{
	}

	virtual ~PackedAnimation()
	{
		for (auto &&animation : animations)
		{
			delete animation;
		}
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

	void endImmediately() override
	{
		for (auto &&animation : animations)
		{
			animation->endImmediately();
		}
	}
};
