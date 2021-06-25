#pragma once
#include "WorldObject.h"

class UpdatableObject : public WorldObject
{
public:
	virtual void update(const float& deltaTime)
	{
	}
};
