#pragma once
#include "CollidableObject.h"
#include "DrawableObject.h"
#include "../model/VAOBuilder.h"

class CollidableDrawableObject : public DrawableObject, public CollidableObject
{
private:
	bool isColliderDrawing = false;

	ModelTransformations colliderTransformations;
	Shader* colliderShader = nullptr;
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();

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
	CollidableDrawableObject(const ColliderCube& colliderCube, const ModelTransformations& transformations, const std::string& shader, const bool& shaderWithGeometry = false)
		:DrawableObject(transformations, shader, shaderWithGeometry),
		 CollidableObject(colliderCube)
	{
		calculateColliderTransformations();
	}

	void checkCollisionAndCorrectPositionIfNeeded(CollidableObject *other)
	{
		if (hasCollision(other))
		{
			CollizionResolver collizionResolver = getCollizionResolver(other);
			offsetPosition( collizionResolver.pushOutVector * collizionResolver.pushOutOffset );
		}
	}

	void setPosition(const glm::vec3& pos) override
	{
		DrawableObject::setPosition(pos);
		colliderCube.setPosition(pos);
		calculateColliderTransformations();
	}

	void offsetPosition(const glm::vec3& offset) override
	{
		DrawableObject::offsetPosition(offset);
		colliderCube.offsetPosition(offset);
		calculateColliderTransformations();
	}

	void setScale(const glm::vec3& scale) override
	{
		DrawableObject::setScale(scale);
		colliderCube.scale(scale);
		calculateColliderTransformations();
	}

	void draw(Renderer* renderer) override
	{
		if(!isColliderDrawing)
		{
			return;
		}

		if (colliderShader == nullptr)
		{
			colliderShader = ShaderLoader::getInstance()->getOrLoadByName("lightCube", FilePaths::getPathToShaderFolderWithTrailingSplitter());
		}

		glm::mat4 cubeModel = transformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", cubeModel);
		shader->setFloatVec3("uColor", glm::vec3(0.8f, 0.0f, 0.0f));
		glBindVertexArray(cubeVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, cubeVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}
};
