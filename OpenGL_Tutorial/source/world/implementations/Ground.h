#pragma once
#include "../../model/ModelTransformations.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"

class Ground : public PositionedObject, public DrawableObject
{
private:
	VOsAndIndices* groundVOsAndIndices = VAOBuilder::getInstance()->getGroundQuad();
	Texture* groundTexture;
	Texture* groundSpecularTexture;
	std::string textureFilename;
	std::string textureSpecularFilename;
public:
	Ground(Renderer* renderer, const std::string& textureFilename, const std::string& textureSpecularFilename, float width)
		:PositionedObject(ModelTransformations{
				glm::vec3(0.f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				90.0f,
				glm::vec3(width)
			}), DrawableObject(renderer, "groundQuad_mixLight"),
		textureFilename(textureFilename), textureSpecularFilename(textureSpecularFilename)
	{
		groundTexture = TexturesLoader::getInstance()->getOrLoad2DTexture(textureFilename, FilePaths::getPathToTexturesFolder());
		groundSpecularTexture = TexturesLoader::getInstance()->getOrLoad2DTexture(textureSpecularFilename, FilePaths::getPathToTexturesFolder());
	}

	void drawAction() override
	{
		// Отрисовка "земли".		
		glm::mat4 groundModel = transformations.createModelMatrixWithTransformations();
		shader->use();
		shader->setFloatMat4("projectionAndView", getAttachedRenderer()->getPV());
		shader->setFloatMat4("model", groundModel);
		shader->setFloat("shininess", getShininess());
		shader->setFloatVec3("viewPos", getAttachedRenderer()->getMainCamera()->position);
		
		int activeLights = 0;
		for(auto *light : getAttachedRenderer()->getLights())
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
