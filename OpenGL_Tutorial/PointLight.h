#pragma once
#include "AttenuatedLight.h"

class PointLight : public AttenuatedLight
{
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0)
		: AttenuatedLight(ambient, diffuse, specular, constant, linear, quadratic, pos_x, pos_y, pos_z)
	{
	}

	virtual void use(const Shader* shader) const override
	{
		std::string lightIndex = std::to_string(id_);
			
		shader->setFloatVec3("pointLights[" + lightIndex + "].position", position_);
		shader->setFloatVec3("pointLights[" + lightIndex + "].ambient", ambient_);
		shader->setFloatVec3("pointLights[" + lightIndex + "].diffuse", diffuse_);
		shader->setFloatVec3("pointLights[" + lightIndex + "].specular", specular_);
		shader->setFloat("pointLights[" + lightIndex + "].constant", constant_);
		shader->setFloat("pointLights[" + lightIndex + "].linear", linear_);
		shader->setFloat("pointLights[" + lightIndex + "].quadratic", quadratic_);
	}

	virtual ~PointLight() = default;
};
