#pragma once
#include "Cube.h"
#include "../light/PositionedLight.h"
#include "../model/ModelTransformations.h"

#include "../model/VAOBuilder.h"
#include "../shader/ShaderLoader.h"
#include "../utility/FilePaths.h"

class LightCube : public Cube
{
private:
	VOsAndIndices* cubeVOsAndIndices = VAOBuilder::getInstance()->getCube();
public:
	PositionedLight *light;

	LightCube(PositionedLight* light, const float scale = 1.0f)
		:Cube( ModelTransformations(light->getPosition()))
		 ,light(light)
	{
		transformations.scale = glm::vec3(scale);

		// Shader специально для данного объекта:
		// TODO: сохранять shader в статик-переменную?
		shader = ShaderLoader::getInstance()->getOrLoadByName("lightCube", FilePaths::getPathToShaderFolderWithTrailingSplitter());
	}

	// TODO: Реализовать virtual-функцию!
	void draw() override
	{
	}

	void draw(const glm::mat4& pv)
	{
		transformations.position = light->getPosition();
		
		glm::mat4 lightCubeModel = transformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", pv);
		shader->setFloatMat4("model", lightCubeModel);
		shader->setFloatVec3("uColor", light->getDiffuse());
		glBindVertexArray(cubeVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, cubeVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}
};
