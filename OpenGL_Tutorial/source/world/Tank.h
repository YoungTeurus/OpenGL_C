#pragma once
#include "CollidableDrawableObject.h"
#include "../model/Model.h"
#include "animations/AnimatedObject.h"

class Tank : public CollidableDrawableObject
{
private:	
	float explosionMagnitude = 0.0f;
	float timeSinceExplosion = 0.0f;

	Model *tankBase;
	Model *tankTurret;

	ModelTransformations turretTransformations;
	
public:
	Tank(Model* tankBase, Model* tankTurret)
	// TODO: ������, ��� �������� � �������������� �������.
		// :CollidableDrawableObject({}, ColliderCube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f), "backpack_mixLightWithExplosion", true),
		:CollidableDrawableObject({}, ColliderCube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f), "backpack_mixLight"),
		 tankBase(tankBase), tankTurret(tankTurret)
	{
	}

	void setPosition(const glm::vec3& pos) override
	{
		// TODO: ���������� �� ����� workaround-�.
		CollidableDrawableObject::setPosition(pos);
		colliderCube.offsetPosition(glm::vec3(0.0f, 1.0f, 0.0f));  // �������� ����������.
		calculateColliderTransformations();
	}
	
	void draw(Renderer* renderer) override
	{
		// ��������� ����������, ���� �����:
		CollidableDrawableObject::draw(renderer);

		// ��������� ������:
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

	void offsetTurretRotationAngleDegrees(const float& angleDegrees)
	{
		turretTransformations.rotationAngleDegrees += angleDegrees;
	}

	float getTurretRotationAngleDegrees() const
	{
		return turretTransformations.rotationAngleDegrees;
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
