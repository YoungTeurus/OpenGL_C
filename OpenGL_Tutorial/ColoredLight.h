#pragma once
#include "BaseLight.h"

class ColoredLight : public BaseLight
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	ColoredLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const std::string& name, const LightType lightType)
		: BaseLight(name, lightType),
			ambient(ambient), diffuse(diffuse), specular(specular)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		BaseLight::setShaderParameters(shader, lightNumber);
		shader->setFloatVec3("lights[" + lightNumber + "].ambient",	ambient);
		shader->setFloatVec3("lights[" + lightNumber + "].diffuse",	diffuse);
		shader->setFloatVec3("lights[" + lightNumber + "].specular",	specular);
	}
};
