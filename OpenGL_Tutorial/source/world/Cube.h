#pragma once
#include <exception>
#include <vector>

#include "../model/ModelTransformations.h"

class Cube
{
protected:
	static unsigned cubeVAO;
	static std::vector<unsigned> indices;
	ModelTransformations cubeTransformations;

public:
	Cube(ModelTransformations transformations)
		:cubeTransformations(transformations)
	{
	}

	virtual ~Cube() = default;

	virtual void draw()
	{
		throw new std::exception("Not implemented!");
	}
};