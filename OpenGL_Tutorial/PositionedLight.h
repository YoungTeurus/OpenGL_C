#pragma once
#include <glm/vec3.hpp>

#include "ColoredLight.h"

class PositionedLight : public ColoredLight
{
protected:
	glm::vec3 position;

public:
	PositionedLight(const glm::vec3 pos,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const std::string& name, const LightType lightType)
		:ColoredLight(ambient, diffuse, specular, name, lightType), position(pos)
	{}

	virtual void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		ColoredLight::setShaderParameters(shader, lightNumber);
		shader->setFloatVec3("lights[" + lightNumber + "].position", position);
	}

	void setPosition(const glm::vec3& position)
	{
		this->position = position;
	}
};
