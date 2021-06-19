#pragma once
#include "DrawableObject.h"
#include "../model/Model.h"

class Tank : public DrawableObject
{
private:	
	float explosionMagnitude = 0.0f;
	float timeSinceExplosion = 0.0f;

	Model *tankBase;
	Model *tankTurret;

	ModelTransformations turretTransformations;
	
public:
	Tank(Model* tankBase, Model* tankTurret)
		:DrawableObject({}, "backpack_mixLightWithExplosion", true),
		 tankBase(tankBase), tankTurret(tankTurret)
	{
	}
	
	void draw(Renderer* renderer) override
	{
		// Отрисовка модели:
		glm::mat4 tankBaseModel = transformations.createModelMatrixWithTransformations();
		
		shader->use();
		shader->setFloat("uExplosionMagnitude", explosionMagnitude);
		shader->setFloat("uTimeSinceExplosion", timeSinceExplosion);
		shader->setFloatMat4("projectionAndView", renderer->getPV());
		shader->setFloatMat4("model", tankBaseModel);
		shader->setFloat("shininess", 64.0f);
		shader->setFloatVec3("viewPos", renderer->getMainCamera().position);
		
		int activeLights = 0;
		for(auto *light : renderer->getLights())
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

	void setTurretRotationAngleDegrees(const float& angleDegrees)
	{
		turretTransformations.rotationAngleDegrees = angleDegrees;
	}

	float getTurretRotationAngleDegrees() const
	{
		return turretTransformations.rotationAngleDegrees;
	}
};
