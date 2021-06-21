#pragma once
#include "CollidableObject.h"
#include "DrawableObject.h"
#include "../../model/ModelTransformations.h"

class CollidableDrawableObject : public CollidableObject, public DrawableObject
{
private:
	bool isColliderDrawing = false;

	ModelTransformations colliderTransformations;
	Shader* colliderShader = nullptr;
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();

protected:
	void calculateColliderTransformations()
	{
		colliderTransformations = ModelTransformations{
				colliderCube.getPosition(),
				glm::vec3(0.0f, 1.0f, 0.0f),
				0.0f,
				colliderCube.getSize()
			};
	}
public:
	CollidableDrawableObject(Renderer* renderer, const ModelTransformations& transformations, const OffsetAndSize& colliderOffsetAndSize, const std::string& shader, const bool& shaderWithGeometry = false)
		:CollidableObject(transformations, colliderOffsetAndSize),
		 DrawableObject(renderer, shader, shaderWithGeometry)
	{
		calculateColliderTransformations();
	}

	void setPosition(glm::vec3 pos) override
	{
		CollidableObject::setPosition(pos);
		calculateColliderTransformations();
	}

	void offsetPosition(const glm::vec3& offset) override
	{
		CollidableObject::offsetPosition(offset);
		calculateColliderTransformations();
	}

	void setScale(const glm::vec3& scale) override
	{
		CollidableObject::setScale(scale);
		calculateColliderTransformations();
	}
	
	void drawAction() override
	{
		if(!isColliderDrawing)
		{
			return;
		}

		if (colliderShader == nullptr)
		{
			colliderShader = ShaderLoader::getInstance()->getOrLoad("collider", FilePaths::getPathToShaderFolderWithTrailingSplitter());
		}

		glm::mat4 cubeModel = colliderTransformations.createModelMatrixWithTransformations();
		colliderShader->use();
		colliderShader->setFloatMat4("projectionAndView", getAttachedRenderer()->getPV());
		colliderShader->setFloatMat4("model", cubeModel);
		glBindVertexArray(cubeVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, cubeVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}

	void setColliderDrawing(const bool& state)
	{
		isColliderDrawing = state;
	}
};
