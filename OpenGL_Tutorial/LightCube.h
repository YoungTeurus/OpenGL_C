#pragma once
#include "Cube.h"
#include "PointLight.h"

class LightCube : public Cube, PointLight
{
	LightCube(const Shader* shader, Material* material,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0,
		const float scale_x = 1, const float scale_y = 1, const float scale_z = 1)
			:Cube(shader, material, pos_x, pos_y, pos_z, scale_x, scale_y, scale_z),
			 PointLight(shader, ambient, diffuse, specular, constant, linear, quadratic, pos_x, pos_y, pos_z)
	{
	}
};

