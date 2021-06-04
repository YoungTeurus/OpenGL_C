#pragma once
#include "BaseLight.h"
#include "PositionedLight.h"

/// <summary>
/// Абстрактный класс затухающего света.
/// От него необходимо наследоваться тем видам источников света,
/// свет от которых становится более слабым с расстоянием.
/// </summary>
class AttenuatedLight : public PositionedLight
{
protected:
	float constant;
	float linear;
	float quadratic;

public:
	AttenuatedLight(const float constant, const float linear, const float quadratic,
		const glm::vec3 pos, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const std::string& name, const LightType lightType)
			:PositionedLight(pos, ambient, diffuse, specular, name, lightType),
				constant(constant), linear(linear), quadratic(quadratic)
	{}

	void setShaderParameters(Shader* shader, std::string lightNumber) override
	{
		PositionedLight::setShaderParameters(shader, lightNumber);
		shader->setFloat("lights[" + lightNumber + "].constant", constant);
		shader->setFloat("lights[" + lightNumber + "].linear", linear);
		shader->setFloat("lights[" + lightNumber + "].quadratic", quadratic);
	}
};