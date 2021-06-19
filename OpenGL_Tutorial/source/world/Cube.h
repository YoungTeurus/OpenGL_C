#pragma once

#include "DrawableObject.h"
#include "PositionedWorldObject.h"
#include "../model/ModelTransformations.h"
#include "../model/VAOBuilder.h"

class Cube : public PositionedWorldObject, public DrawableObject
{
protected:
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();
public:
	Cube(ModelTransformations transformations, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:PositionedWorldObject(transformations), DrawableObject(shaderName, shaderWithGeometry)
	{
	}

	virtual ~Cube() = default;
};
