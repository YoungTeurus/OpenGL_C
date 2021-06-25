#pragma once
#include "ParticleGenerator.h"
#include "../../model/ModelsLoader.h"
#include "../../model/ModelTransformations.h"
#include "../../renderer/Renderer.h"
#include "../animations/AnimatedObject.h"
#include "../interfaces/CollidableDrawableObject.h"

class TankBullet : public CollidableDrawableObject, public AnimatedObject<TankBullet>
{
private:	
	float explosionMagnitude = 0.0f;
	float timeSinceExplosion = 0.0f;

	Model *bulletModel;
	ParticleGenerator *particleGenerator;

	glm::vec3 velocity;
	float life = 10.0f;
	
public:
	TankBullet(Renderer* renderer, ModelTransformations transformations, const glm::vec3& velocity)
		:CollidableDrawableObject(renderer, transformations, {glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f)}, "model_mixLightWithExplosion", true),
		 bulletModel(ModelsLoader::getInstance()->getOrLoad("tank/base.obj", true)), velocity(velocity)
	{
		particleGenerator = new ParticleGenerator(renderer, transformations, 50, 3, 0.01, 3.0f, 1.0f, glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f), -velocity, glm::vec3(0.3f));
		particleGenerator->activate();
	}

	~TankBullet()
	{
		delete particleGenerator;
	}
	
	void drawAction() override
	{
		// Отрисовка коллайдера, если нужно:
		CollidableDrawableObject::drawAction();

		// Отрисовка модели:
		glm::mat4 tankBaseModel = transformations.createModelMatrixWithTransformations();
		
		shader->use();
		shader->setFloat("uExplosionMagnitude", explosionMagnitude);
		shader->setFloat("uTimeSinceExplosion", timeSinceExplosion);
		shader->setFloatMat4("projectionAndView", getAttachedRenderer()->getPV());
		shader->setFloatMat4("model", tankBaseModel);
		shader->setFloat("shininess", getShininess());
		shader->setFloatVec3("viewPos", getAttachedRenderer()->getMainCamera()->position);
		
		int activeLights = 0;
		for(auto *light : getAttachedRenderer()->getLights())
		{
			activeLights += light->useAndReturnSuccess(shader, activeLights);
		}
		
		shader->setInt("lightsCount", activeLights);
		
		bulletModel->draw(*shader);

		particleGenerator->draw();
	}

	void update(const float& deltaTime) override
	{
		AnimatedObject::update(deltaTime);
		particleGenerator->update(deltaTime);

		offsetPosition(velocity * deltaTime);
		life -= deltaTime;
		if (life <= 0)
		{
			this->setVisible(false);
			particleGenerator->deactivate();
		}
	}
};
