#pragma once
#include "WorldObject.h"

class ShaderedWorldObject : public WorldObject
{
protected:
	const Shader *_shader = nullptr;

	ShaderedWorldObject(const Shader* shader,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0)
		: WorldObject(pos_x, pos_y, pos_z), _shader(shader)
	{
	}

	virtual ~ShaderedWorldObject() = default;
};
