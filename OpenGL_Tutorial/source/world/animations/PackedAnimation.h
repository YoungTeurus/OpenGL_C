#pragma once
#include <vector>

#include "Animation.h"

template<class T>
class PackedAnimation : public Animation<T>
{
private:
	std::vector<Animation<T>*> animations;
public:
	PackedAnimation(T *object, const float& length)
		:Animation<T>(object, length)
	{
	}

	virtual ~PackedAnimation()
	{
		for (auto &&animation : animations)
		{
			delete animation;
		}
	}

	PackedAnimation* addAnimation(Animation<T>* animation)
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
