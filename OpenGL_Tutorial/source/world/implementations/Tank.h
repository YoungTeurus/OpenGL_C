#pragma once
#include "TankBullet.h"
#include "../../model/ModelsLoader.h"
#include "../../model/ModelTransformations.h"
#include "../../renderer/Renderer.h"
#include "../animations/AnimatedObject.h"
#include "../interfaces/CollidableDrawableObject.h"

class Tank : public CollidableDrawableObject, public AnimatedObject<Tank>
{
private:	
	float explosionMagnitude = 0.0f;
	float timeSinceExplosion = 0.0f;

	Model *tankBase;
	Model *tankTurret;

	ModelTransformations turretTransformations;
	
public:
	Tank(Renderer* renderer, ModelTransformations transformations)
		:CollidableDrawableObject(renderer, transformations, {glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f)}, "model_mixLightWithExplosion", true),
		 tankBase(ModelsLoader::getInstance()->getOrLoad("tank/base.obj", true)), tankTurret(ModelsLoader::getInstance()->getOrLoad("tank/turret.obj", true))
	{
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
		
		tankBase->draw(*shader);
		
		glm::mat4 tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = turretTransformations.getModelWithAppliedTransformations(tankTurretModel);
		shader->setFloatMat4("model", tankTurretModel);
		tankTurret->draw(*shader);
	}

	TankBullet* createBullet() const
	{
		const float bulletDegree = getTurretAbsoluteRotationAngleDegrees();

		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(bulletDegree), glm::vec3(0.0f, 1.0f, 0.0f));
		
		const glm::vec3 bulletVelocity = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		TankBullet* bullet = new TankBullet(getAttachedRenderer(),
			ModelTransformations{getPosition(),
			glm::vec3(0.0f, 1.0f, 0.0f),
			bulletDegree, glm::vec3(1.0f)},
			bulletVelocity);
		
		return bullet;
	}

	void setTurretRotationAngleDegrees(float angleDegrees)
	{
		turretTransformations.rotationAngleDegrees = angleDegrees;
	}

	void offsetTurretRotationAngleDegrees(const float& angleDegrees)
	{
		turretTransformations.rotationAngleDegrees += angleDegrees;
	}

	float getTurretRotationAngleDegrees() const
	{
		return turretTransformations.rotationAngleDegrees;
	}

	float getTurretAbsoluteRotationAngleDegrees() const
	{
		return clampAngleTo360( getRotationAngleDegrees() + getTurretRotationAngleDegrees() );
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
