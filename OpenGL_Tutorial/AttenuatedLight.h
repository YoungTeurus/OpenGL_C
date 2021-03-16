#pragma once
#include "Light.h"

// Источник света с затуханием (точечный, прожекторный)
class AttenuatedLight : public Light
{
protected:
	float constant_;
	float linear_;
	float quadratic_;
public:
	AttenuatedLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0) : Light(ambient, diffuse, specular, pos_x, pos_y, pos_z), constant_(constant), linear_(linear), quadratic_(quadratic)
	{
	}

	virtual ~AttenuatedLight() = default;
};

