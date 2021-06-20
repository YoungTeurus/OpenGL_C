#pragma once
#include "WorldObject.h"
#include "../../model/ModelTransformations.h"

class PositionedObject : public WorldObject
{
private:	
	// Приводит угол в диапазон [0, 360)
	void clampAngleDegrees()
	{
		transformations.rotationAngleDegrees = clampAngleTo360(transformations.rotationAngleDegrees);
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

	virtual std::string toString()
	{
		return string("posObj{") +
			"(" + to_string(transformations.position.x) + "," + to_string(transformations.position.y) + "," + to_string(transformations.position.z) + ")" +
			"}"
		; 
	}

	static float clampAngleTo360(const float& angle)
	{
		if (angle < 0.0f)
		{
			float absedAngleDegrees = abs(angle);
			int fullCircles = (int)absedAngleDegrees / 360.0f;
			absedAngleDegrees = absedAngleDegrees - fullCircles * 360.0f;
			return 360.0f - absedAngleDegrees;
		}
		if (angle >= 360.0f)
		{
			int fullCircles = (int)angle / 360.0f;
			return angle - fullCircles * 360.0f;
		}
		return angle;
	}
};
