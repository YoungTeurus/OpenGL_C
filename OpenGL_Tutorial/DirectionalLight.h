#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 direction_;
public:
	static bool has_directional_light;
	
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		glm::vec3 direction)
			: Light(ambient, diffuse, specular), direction_(direction)
	{
		has_directional_light = true;
	}

	void use(const Shader* shader) const override
	{
		shader->setBool("hasDirLight", has_directional_light);
		// DirLight
		shader->setFloatVec3("dirLight.direction", direction_);
		shader->setFloatVec3("dirLight.ambient", ambient_);
		shader->setFloatVec3("dirLight.diffuse", diffuse_);
		shader->setFloatVec3("dirLight.specular", specular_);
	}

	virtual ~DirectionalLight() = default;
};

bool DirectionalLight::has_directional_light = false;