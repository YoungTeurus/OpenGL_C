#pragma once
#include "../../model/ModelTransformations.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"

class Ground : public PositionedObject, public DrawableObject
{
private:
	VOsAndIndices* groundVOsAndIndices = VAOBuilder::getInstance()->getGroundQuad();
	Texture* groundTexture = nullptr;
	Texture* groundSpecularTexture = nullptr;
	std::string textureFilename;
	std::string textureSpecularFilename;
public:
	Ground(const std::string& textureFilename, const std::string& textureSpecularFilename, float width)
		:PositionedObject(ModelTransformations{
				glm::vec3(0.f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				90.0f,
				glm::vec3(width)
			}), DrawableObject("groundQuad_mixLight"),
		textureFilename(textureFilename), textureSpecularFilename(textureSpecularFilename)
	{
	}

	void drawAction(Renderer* renderer) override
	{
		// Отрисовка "земли".

		// Проверка загруженности текстур:
		if(groundTexture == nullptr)
		{
			groundTexture = renderer->getTexturesLoader()->getOrLoad2DTexture(textureFilename, FilePaths::getPathToTexturesFolder());
		}
		if (groundSpecularTexture == nullptr)
		{
			groundSpecularTexture = renderer->getTexturesLoader()->getOrLoad2DTexture(textureSpecularFilename, FilePaths::getPathToTexturesFolder());
		}
		
		glm::mat4 groundModel = transformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", groundModel);
		shader->setFloat("shininess", 64.0f);
		shader->setFloatVec3("viewPos", renderer->getMainCamera().position);
		
		int activeLights = 0;
		for(auto *light : renderer->getLights())
		{
			activeLights += light->useAndReturnSuccess(shader, activeLights);
		}
		shader->setInt("lightsCount", activeLights);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundTexture->getId());
		shader->setInt("texture_diffuse", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, groundSpecularTexture->getId());
		shader->setInt("texture_specular", 1);
		
		glBindVertexArray(groundVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, groundVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}
};
