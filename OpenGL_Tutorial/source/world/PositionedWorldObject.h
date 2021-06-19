#pragma once
#include "WorldObject.h"
#include "../model/ModelTransformations.h"

class PositionedWorldObject : public WorldObject
{
protected:
	ModelTransformations transformations;
public:
	PositionedWorldObject(ModelTransformations transformations)
		:transformations(transformations)
	{
	}

	virtual ~PositionedWorldObject() = default;
	
	virtual void setPosition(const glm::vec3& pos)
	{
		transformations.position = pos;
	}

	virtual void offsetPosition(const glm::vec3& offset)
	{
		transformations.position += offset;
	}

	glm::vec3 getPosition() const
	{
		return transformations.position;
	}

	void setRotationVector(const glm::vec3& rotationVector)
	{
		transformations.rotationVector = rotationVector;
	}

	glm::vec3 getRotationVector() const
	{
		return transformations.rotationVector;
	}

	void setRotationAngleDegrees(const float& angleDegrees)
	{
		transformations.rotationAngleDegrees = angleDegrees;
	}

	void offsetRotationAngleDegrees(const float& angleDegrees)
	{
		transformations.rotationAngleDegrees += angleDegrees;
	}

	float getRotationAngleDegrees() const
	{
		return transformations.rotationAngleDegrees;
	}

	void setScale(const float& scale)
	{
		setScale(glm::vec3(scale));
	}

	virtual void setScale(const glm::vec3& scale)
	{
		transformations.scale = scale;
	}

	glm::vec3 getScale() const
	{
		return transformations.scale;
	}
};
