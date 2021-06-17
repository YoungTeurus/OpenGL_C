#pragma once
#include "../SceneObject.h"

class Tank : public SceneObject
{
public:
	Shader* assimpModelWithLightsAndExplosionShader;
	
	Model *baseModel;
	Model *turretModel;

	SceneObjectTransformations baseTransformations;
	SceneObjectTransformations turretTransformations;

	Tank(const string& name, Shader* assimpModelWithLightsAndExplosionShader, Model* tankBase, Model* tankTurret, const SceneObjectTransformations& baseTransformations)
		:Tank(name, assimpModelWithLightsAndExplosionShader, tankBase, tankTurret, baseTransformations, {})
	{
		
	}
	
	Tank(const string& name, Shader* assimpModelWithLightsAndExplosionShader, Model* tankBase, Model* tankTurret, const SceneObjectTransformations& baseTransformations, const SceneObjectTransformations& turretTransformations)
		:SceneObject(name),
		 assimpModelWithLightsAndExplosionShader(assimpModelWithLightsAndExplosionShader),
		 baseModel(tankBase), turretModel(tankTurret), baseTransformations(baseTransformations), turretTransformations(turretTransformations)
	{
	}
	
	void draw() const override
	{
		glm::mat4 tankBaseModel = baseTransformations.createModelMatrixWithTransformations();
		
		assimpModelWithLightsAndExplosionShader->use();
		assimpModelWithLightsAndExplosionShader->setFloat("uExplosionMagnitude", 0.0f);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankBaseModel);
		assimpModelWithLightsAndExplosionShader->setFloat("shininess", 64.0f);
		
		baseModel->draw(*assimpModelWithLightsAndExplosionShader);
		
		glm::mat4 tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = turretTransformations.getModelWithAppliedTransformations(tankTurretModel);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankTurretModel);
		turretModel->draw(*assimpModelWithLightsAndExplosionShader);
	}
};
