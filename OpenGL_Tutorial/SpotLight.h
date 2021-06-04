#pragma once
#include "AttenuatedLight.h"

class SpotLight : public AttenuatedLight
{
private:
	float cutOff;
	glm::vec3 direction;
public:
	SpotLight(const float& cutOff, const glm::vec3& direction,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float constant, const float linear, const float quadratic, const glm::vec3 pos, const std::string& name)
		: AttenuatedLight(constant, linear, quadratic, pos, ambient, diffuse, specular, name, LightType::Spot),
		cutOff(cutOff), direction(direction)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		AttenuatedLight::setShaderParameters(shader, lightNumber);
		shader->setFloat("lights[" + lightNumber + "].cutOff",	cutOff);
		shader->setFloatVec3("lights[" + lightNumber + "].direction",	direction);
	}

	void setDirection(glm::vec3 direction)
	{
		this->direction = direction;
	}
};
