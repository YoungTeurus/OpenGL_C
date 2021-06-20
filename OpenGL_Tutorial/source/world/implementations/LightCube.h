#pragma once
#include "Cube.h"

class LightCube : public Cube
{
public:
	PositionedLight *light;

	LightCube(PositionedLight* light, const float scale = 1.0f)
		:Cube( ModelTransformations(light->getPosition()), "lightCube"),
		 light(light)
	{
		transformations.scale = glm::vec3(scale);
	}

	void drawAction(Renderer* renderer) override
	{
		if (! light->getIsActive())
		{
			return;
		}
		transformations.position = light->getPosition();
		
		glm::mat4 lightCubeModel = transformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", lightCubeModel);
		shader->setFloatVec3("uColor", light->getDiffuse());
		glBindVertexArray(cubeVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, cubeVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}
};
