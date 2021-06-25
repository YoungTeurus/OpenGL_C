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
	BrickWall(Renderer* renderer,const ModelTransformations& transformations)
		:CollidableDrawableObject(renderer, transformations, {glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f)}, "model_mixLightWithExplosion", true),
		 model(ModelsLoader::getInstance()->getOrLoad("walls/brick.obj", true))
	{
		setShininess(2.0f);
	}
	
	void drawAction() override
	{
		// Отрисовка коллайдера, если нужна:
		CollidableDrawableObject::drawAction();

		// Отрисовка модели:
		glm::mat4 wallModel = transformations.createModelMatrixWithTransformations();
		
		shader->use();
		shader->setFloat("uExplosionMagnitude", explosionMagnitude);
		shader->setFloat("uTimeSinceExplosion", timeSinceExplosion);
		shader->setFloatMat4("projectionAndView", getAttachedRenderer()->getPV());
		shader->setFloatMat4("model", wallModel);
		shader->setFloat("shininess", getShininess());
		shader->setFloatVec3("viewPos", getAttachedRenderer()->getMainCamera()->position);
		
		int activeLights = 0;
		for(auto *light : getAttachedRenderer()->getLights())
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
