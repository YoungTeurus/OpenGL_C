#pragma once

#include "WorldObject.h"
#include "../model/ModelTransformations.h"
#include "../shader/Shader.h"

class DrawableObject : public WorldObject
{
protected:
	Shader* shader;
	ModelTransformations transformations;
public:
	DrawableObject(ModelTransformations transformations)
		:transformations(transformations)
	{
	}
	
	virtual ~DrawableObject() = default;

	virtual void draw() = 0;
};
