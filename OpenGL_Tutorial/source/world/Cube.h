#pragma once
#include <vector>


#include "DrawableObject.h"
#include "../model/ModelTransformations.h"

class Cube : public DrawableObject
{
protected:

public:
	Cube(ModelTransformations transformations)
		:DrawableObject(transformations)
	{
	}

	virtual ~Cube() = default;
};