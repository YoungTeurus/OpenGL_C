#pragma once
#include "ColoredLight.h"

class AmbientLight : public ColoredLight
{
public:
	AmbientLight(const glm::vec3& ambient, const std::string& name)
		: ColoredLight(ambient, glm::vec3(0.0f), glm::vec3(0.0f), name, LightType::Ambient)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		ColoredLight::setShaderParameters(shader, lightNumber);
	}
};
