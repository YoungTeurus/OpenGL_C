#pragma once
#include "../../model/ModelTransformations.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"


class Cube : public PositionedObject, public DrawableObject
{
protected:
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();
public:
	Cube(ModelTransformations transformations, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:PositionedObject(transformations), DrawableObject(shaderName, shaderWithGeometry)
	{
	}

	virtual ~Cube() = default;
};
