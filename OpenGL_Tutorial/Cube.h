#pragma once
#include <vector>
#include <glm/vec3.hpp>


#include "StaticFigures.h"
#include "classes/Shader.h"

class Cube
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::vec3 color;

	unsigned VAO;
	unsigned VBO;
	unsigned EBO;

	static unsigned cubeVAO;
	static std::vector<unsigned> indices;

	Cube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color)
		: position(position), scale(scale), rotation(rotation), color(color)
	{
	}

	inline void draw(const Shader& shader) const
	{
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};

std::vector<unsigned> Cube::indices = StaticFigures::getCubeIndices();
unsigned Cube::cubeVAO = StaticFigures::getLoadedCubeVAO();