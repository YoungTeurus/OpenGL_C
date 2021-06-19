#pragma once
#include <vector>

typedef unsigned VAO, VBO, EBO;
typedef unsigned Indice;

class VOsAndIndices
{
public:
	VAO vao;
	VBO vbo;
	EBO ebo;
	std::vector<Indice> indices;


	VOsAndIndices(VAO vao, VBO vbo, EBO ebo, const std::vector<Indice>& indices)
		: vao(vao),
		  vbo(vbo),
		  ebo(ebo),
		  indices(indices)
	{
	}
};