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
	CollidableDrawableObject(const ModelTransformations& transformations, const ColliderCube& colliderCube, const std::string& shader, const bool& shaderWithGeometry = false)
		:CollidableObject(transformations, colliderCube),
		 DrawableObject(shader, shaderWithGeometry)
	{
		calculateColliderTransformations();
	}

	void setPosition(const glm::vec3& pos) override
	{
		CollidableObject::setPosition(pos);
		colliderCube.setPosition(pos);
		calculateColliderTransformations();
	}

	void offsetPosition(const glm::vec3& offset) override
	{
		CollidableObject::offsetPosition(offset);
		colliderCube.offsetPosition(offset);
		calculateColliderTransformations();
	}

	void setScale(const glm::vec3& scale) override
	{
		CollidableObject::setScale(scale);
		colliderCube.scale(scale);
		calculateColliderTransformations();
	}
	
	void drawAction(Renderer* renderer) override
	{
		if(!isColliderDrawing)
		{
			return;
		}

		if (colliderShader == nullptr)
		{
			colliderShader = ShaderLoader::getInstance()->getOrLoad("lightCube", FilePaths::getPathToShaderFolderWithTrailingSplitter());
		}

		glm::mat4 cubeModel = colliderTransformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", cubeModel);
		shader->setFloatVec3("uColor", glm::vec3(0.8f, 0.0f, 0.0f));
		glBindVertexArray(cubeVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, cubeVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}

	void setColliderDrawing(const bool& state)
	{
		isColliderDrawing = state;
	}
};
