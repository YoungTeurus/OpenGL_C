#pragma once
#include "DrawableObject.h"
#include "UpdatableObject.h"

class DrawableUpdatableObject : public DrawableObject, public UpdatableObject
{
public:
	DrawableUpdatableObject(Renderer* renderer, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:DrawableObject(renderer, shaderName, shaderWithGeometry)
	{
	}
};
