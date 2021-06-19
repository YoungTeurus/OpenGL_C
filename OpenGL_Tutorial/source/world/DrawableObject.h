#pragma once

#include "WorldObject.h"
#include "../model/ModelTransformations.h"
#include "../renderer/Renderer.h"
#include "../shader/Shader.h"
#include "../shader/ShaderLoader.h"
#include "../utility/FilePaths.h"

class DrawableObject : public WorldObject
{
protected:
	Shader* shader;
	ModelTransformations transformations;
public:
	DrawableObject(ModelTransformations transformations, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:transformations(transformations),
		 shader( ShaderLoader::getInstance()->getOrLoadByName(shaderName, FilePaths::getPathToShaderFolderWithTrailingSplitter(), shaderWithGeometry) )
	{
	}

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
	
	virtual ~DrawableObject() = default;

	virtual void draw(Renderer* renderer) = 0;
};
