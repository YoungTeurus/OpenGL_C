#pragma once
#include "DrawableObject.h"
#include "../model/VAOBuilder.h"
#include "../model/VOsAndIndices.h"
#include "../shader/ShaderLoader.h"

class Ground : public DrawableObject
{
private:
	VOsAndIndices* groundVOsAndIndices = VAOBuilder::getInstance()->getGroundQuad();
public:
	Ground(float width)
		:DrawableObject(
			ModelTransformations{
				glm::vec3(0.f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				90.0f,
				glm::vec3(width)
			},
			"groundQuad_mixLight"
		)
	{
	}

	void draw(Renderer* renderer) override
	{
		// ��������� "�����".
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
		glBindTexture(GL_TEXTURE_2D, groundTextureID);
		shader->setInt("texture_diffuse", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, groundSpecularTextureID);
		shader->setInt("texture_specular", 1);
		
		glBindVertexArray(groundVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, groundVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
	}
};