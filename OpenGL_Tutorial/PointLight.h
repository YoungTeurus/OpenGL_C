#pragma once
#include "AttenuatedLight.h"
#include "ColoredLight.h"

class PointLight : public AttenuatedLight
{
public:
	PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float constant, const float linear, const float quadratic, const glm::vec3 pos, const std::string& name)
		: AttenuatedLight(constant, linear, quadratic, pos, ambient, diffuse, specular, name, LightType::Point)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		AttenuatedLight::setShaderParameters(shader, lightNumber);
	}
};