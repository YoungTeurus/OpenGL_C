#pragma once
#include "WorldObject.h"
#include "../../model/ModelTransformations.h"

class PositionedObject : public WorldObject
{
private:
	// Приводит угол в диапазон [0, 360)
	void clampAngleDegrees()
	{
		if (transformations.rotationAngleDegrees < 0.0f)
		{
			float absedAngleDegrees = abs(transformations.rotationAngleDegrees);
			int fullCircles = (int)absedAngleDegrees / 360.0f;
			absedAngleDegrees = absedAngleDegrees - fullCircles * 360.0f;
			transformations.rotationAngleDegrees = 360.0f - absedAngleDegrees;
		} else if (transformations.rotationAngleDegrees >= 360.0f)
		{
			int fullCircles = (int)transformations.rotationAngleDegrees / 360.0f;
			transformations.rotationAngleDegrees = transformations.rotationAngleDegrees - fullCircles * 360.0f;
		}
	}
protected:
	ModelTransformations transformations;
public:
	PositionedObject(const ModelTransformations& transformations)
		:transformations(transformations)
	{
	}

	virtual ~PositionedObject() = default;
	
	virtual void setPosition(glm::vec3 pos)
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

	void setRotationAngleDegrees(float angleDegrees)
	{
		transformations.rotationAngleDegrees = angleDegrees;
		clampAngleDegrees();
	}

	void offsetRotationAngleDegrees(const float& angleDegrees)
	{
		transformations.rotationAngleDegrees += angleDegrees;
		clampAngleDegrees();
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
