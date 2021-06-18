#pragma once
#include "../light/PositionedLight.h"

class LightCube
{
public:
	PositionedLight *light;

	LightCube(PositionedLight* light)
		:light(light)
	{
	}
};
