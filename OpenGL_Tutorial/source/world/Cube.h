#pragma once

#include "DrawableObject.h"
#include "../model/ModelTransformations.h"
#include "../model/VAOBuilder.h"

class Cube : public DrawableObject
{
protected:
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();
public:
	Cube(ModelTransformations transformations, const std::string& shaderName)
		:DrawableObject(transformations, shaderName)
	{
	}

	virtual ~Cube() = default;
};
