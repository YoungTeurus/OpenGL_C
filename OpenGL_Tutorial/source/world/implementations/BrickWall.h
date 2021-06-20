#pragma once
#include "../../model/ModelsLoader.h"
#include "../../model/ModelTransformations.h"
#include "../../renderer/Renderer.h"
#include "../animations/AnimatedObject.h"
#include "../interfaces/CollidableDrawableObject.h"

class BrickWall : public CollidableDrawableObject, public AnimatedObject<BrickWall>
{
private:	
	float explosionMagnitude = 0.0f;
	float timeSinceExplosion = 0.0f;

	Model *model;
	
public:
	BrickWall(const ModelTransformations& transformations)
		:CollidableDrawableObject(transformations, {glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f)}, "model_mixLightWithExplosion", true),
		 model(ModelsLoader::getInstance()->getOrLoad("walls/brick.obj", true))
	{
	}

	void setPosition(glm::vec3 pos) override
	{
		// TODO: избавиться от этого workaround-а.
		CollidableDrawableObject::setPosition(pos);
		colliderCube.offsetPosition(glm::vec3(0.0f, 1.0f, 0.0f));  // Поправка коллайдера.
		calculateColliderTransformations();
	}
	
	void drawAction(Renderer* renderer) override
	{
		// Отрисовка коллайдера, если нужна:
		CollidableDrawableObject::drawAction(renderer);

		// Отрисовка модели:
		glm::mat4 wallModel = transformations.createModelMatrixWithTransformations();
		
		shader->use();
		shader->setFloat("uExplosionMagnitude", explosionMagnitude);
		shader->setFloat("uTimeSinceExplosion", timeSinceExplosion);
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", wallModel);
		shader->setFloat("shininess", 64.0f);
		shader->setFloatVec3("viewPos", renderer->getMainCamera()->position);
		
		int activeLights = 0;
		for(auto *light : renderer->getLights())
		{
			activeLights += light->useAndReturnSuccess(shader, activeLights);
		}
		
		shader->setInt("lightsCount", activeLights);
		
		model->draw(*shader);
	}

	float getExplosionMagnitude() const
	{
		return explosionMagnitude;
	}

	void setExplosionMagnitude(float explosionMagnitude)
	{
		this->explosionMagnitude = explosionMagnitude;
	}

	float getTimeSinceExplosion() const
	{
		return timeSinceExplosion;
	}

	void setTimeSinceExplosion(float timeSinceExplosion)
	{
		this->timeSinceExplosion = timeSinceExplosion;
	}
};
