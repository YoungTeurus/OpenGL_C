#pragma once
#include "WorldObject.h"

class UpdatableObject : WorldObject
{
public:
	virtual void update(const float& deltaTime)
	{
	}
};
