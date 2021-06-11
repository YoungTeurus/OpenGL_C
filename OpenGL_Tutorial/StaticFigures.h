#pragma once
#include <glad/glad.h>
#include <vector>

class StaticFigures
{
public:
	inline static std::vector<float> getCubeVertexesWithNormalsAndUV()
	{
		return std::vector<float>
		{
		//	  X		 Y		 Z		  Nx	 Ny		 Nz		 U	   V
		// Передняя грань (z: +1)
			 1.0f,	1.0f,	1.0f,	 0.0f,	0.0f,	1.0f,	1.0f, 1.0f, // верхний правый
			-1.0f,	1.0f,	1.0f,	 0.0f,	0.0f,	1.0f,	0.0f, 1.0f, // верхний левый
			-1.0f, -1.0f,	1.0f,	 0.0f,	0.0f,	1.0f,	0.0f, 0.0f, // нижний левый
			 1.0f, -1.0f,	1.0f,	 0.0f,	0.0f,	1.0f,	1.0f, 0.0f, // нижний правый
		// Правая грань (x: +1)
			 1.0f,	1.0f,  -1.0f,	 1.0f,	0.0f,	0.0f,	1.0f, 1.0f, // верхний правый
			 1.0f,	1.0f,	1.0f,	 1.0f,	0.0f,	0.0f,	0.0f, 1.0f, // верхний левый
			 1.0f, -1.0f,	1.0f,	 1.0f,	0.0f,	0.0f,	0.0f, 0.0f, // нижний левый
			 1.0f, -1.0f,  -1.0f,	 1.0f,	0.0f,	0.0f,	1.0f, 0.0f, // нижний правый
		// Верхняя грань (y: +1)
			 1.0f,	1.0f,  -1.0f,	 0.0f,	1.0f,	0.0f,	1.0f, 1.0f, // верхний правый
			-1.0f,	1.0f,  -1.0f,	 0.0f,	1.0f,	0.0f,	0.0f, 1.0f, // верхний левый
			-1.0f,	1.0f,   1.0f,	 0.0f,	1.0f,	0.0f,	0.0f, 0.0f, // нижний левый
			 1.0f,	1.0f,   1.0f,	 0.0f,	1.0f,	0.0f,	1.0f, 0.0f, // нижний правый
		// Левая грань (x: -1)
			-1.0f,	1.0f,   1.0f,	-1.0f,	0.0f,	0.0f,	1.0f, 1.0f, // верхний правый
			-1.0f,	1.0f,  -1.0f,	-1.0f,	0.0f,	0.0f,	0.0f, 1.0f, // верхний левый
			-1.0f, -1.0f,  -1.0f,	-1.0f,	0.0f,	0.0f,	0.0f, 0.0f, // нижний левый
			-1.0f, -1.0f,   1.0f,	-1.0f,	0.0f,	0.0f,	1.0f, 0.0f, // нижний правый
		// Нижняя грань (y: -1)
			 1.0f, -1.0f,   1.0f,	 0.0f, -1.0f,	0.0f,	1.0f, 1.0f, // верхний правый
			-1.0f, -1.0f,   1.0f,	 0.0f, -1.0f,	0.0f,	0.0f, 1.0f, // верхний левый
			-1.0f, -1.0f,  -1.0f,	 0.0f, -1.0f,	0.0f,	0.0f, 0.0f, // нижний левый
			 1.0f, -1.0f,  -1.0f,	 0.0f, -1.0f,	0.0f,	1.0f, 0.0f, // нижний правый
		// Задняя грань (z: -1)
			-1.0f,	1.0f,  -1.0f,	 0.0f,	0.0f,  -1.0f,	1.0f, 1.0f, // верхний правый
			 1.0f,	1.0f,  -1.0f,	 0.0f,	0.0f,  -1.0f,	0.0f, 1.0f, // верхний левый
			 1.0f, -1.0f,  -1.0f,	 0.0f,	0.0f,  -1.0f,	0.0f, 0.0f, // нижний левый
			-1.0f, -1.0f,  -1.0f,	 0.0f,	0.0f,  -1.0f,	1.0f, 0.0f, // нижний правый
        };
	}

	inline static std::vector<unsigned> getCubeIndices()
	{
		return std::vector<unsigned>{
			0,   1,  2,     0,  2,  3,
			4,   5,  6,     4,  6,  7,
			8,   9, 10,     8, 10, 11,
			12, 13, 14,    12, 14, 15,
			16, 17, 18,    16, 18, 19,
			20, 21, 22,    20, 22, 23
		};
	}

	inline static std::vector<float> getQuadVertexesWithUV()
	{
		return std::vector<float>
		{
		//	  X		 Y	   U	 V
			-1.0f,  1.0f, 0.0f, 1.0f,	// верхний левый
			-1.0f, -1.0f, 0.0f, 0.0f,	// нижний левый
			 1.0f, -1.0f, 1.0f, 0.0f,	// нижний правый
			 1.0f,  1.0f, 1.0f, 1.0f	// верхний правый
        };
	}

	inline static std::vector<unsigned> getQuadIndices()
	{
		return std::vector<unsigned>{
			0, 1, 2,
			0, 2, 3
		};
	}

	inline static unsigned getLoadedCubeVAO()
	{
		using namespace std;
		unsigned VAO = 0, VBO = 0, EBO = 0;

		// vector<float> cubeVertexData = getCubeVertexesWithNormalsAndUV();
		// vector<unsigned> cubeIndicesData = getCubeIndices();
		// 
		// glGenVertexArrays(1, &VAO);
		// glGenBuffers(1, &VBO);
		// glGenBuffers(1, &EBO);
		// 
		// glBindVertexArray(VAO);
		// glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertexData.size(), cubeVertexData.data(), GL_STATIC_DRAW);
		// 
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * cubeIndicesData.size(), cubeIndicesData.data(), GL_STATIC_DRAW);
		// 
		// // layout (location = 0) in vec3 inFragPosition;
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * cubeVertexData.size(), (void*)0);
		// // layout (location = 1) in vec3 inNormal;
		// glEnableVertexAttribArray(1);
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * cubeVertexData.size(), (void*)(3 * sizeof(float)));
		// // layout (location = 2) in vec2 inTexCoords;
		// glEnableVertexAttribArray(2);
		// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * cubeVertexData.size(), (void*)(6 * sizeof(float)));
		// 
		// glBindVertexArray(0);
		return VAO;
	}
};
