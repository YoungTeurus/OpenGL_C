#pragma once
#include "../../model/ModelTransformations.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"


class Cube : public PositionedObject, public DrawableObject
{
protected:
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();
public:
	Cube(Renderer* renderer, ModelTransformations transformations, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:PositionedObject(transformations), DrawableObject(renderer, shaderName, shaderWithGeometry)
	{
	}

	virtual ~Cube() = default;
};
