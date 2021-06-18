#pragma once
#include "../light/PositionedLight.h"
#include "../model/ModelTransformations.h"

class LightCube
{
public:
	PositionedLight *light;
	ModelTransformations cubeTransformations;

	LightCube(PositionedLight* light, const float scale = 1.0f)
		:light(light), cubeTransformations({light->getPosition()})
	{
		cubeTransformations.scale = glm::vec3(scale);
	}

	void draw(Shader *lightCubeShader)
	{
		// ModelTransformations lightCubeTransformation = {
		// 		lightCube->light->getPosition()
		// 	};
		// glm::mat4 lightCubeModel = lightCubeTransformation.createModelMatrixWithTransformations();
		// lightCubeShader->use();
		// lightCubeShader->setFloatMat4("projectionAndView", pv);
		// lightCubeShader->setFloatMat4("model", lightCubeModel);
		// lightCubeShader->setFloatVec3("uColor", lightCube->light->getDiffuse());
		// glBindVertexArray(cubeVAO);
		// glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
	}
};
