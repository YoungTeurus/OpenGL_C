#pragma once
#include "PositionedLight.h"
#include "ColoredLight.h"

class DirectionalLight : public ColoredLight
{
private:
	glm::vec3 direction;
public:
	DirectionalLight(const glm::vec3& direction,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const std::string& name)
		: ColoredLight(ambient, diffuse, specular, name, LightType::Directional),
		direction(direction)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		ColoredLight::setShaderParameters(shader, lightNumber);
		shader->setFloatVec3("lights[" + lightNumber + "].direction",	direction);
	}
};