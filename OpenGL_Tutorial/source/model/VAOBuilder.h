#pragma once
#include <map>
#include <string>

#include "StaticFigures.h"
#include "VOsAndIndices.h"

class VAOBuilder
{
private:

	static VAOBuilder* instance;
	
	std::map<std::string, VOsAndIndices*>* loadedVOs;

	VAOBuilder()
	{
		loadedVOs = new std::map<std::string, VOsAndIndices*>();
	}

	void addInLoaded(const std::string& name, VOsAndIndices* vOsAndIndices)
	{
		loadedVOs->insert(std::pair<std::string, VOsAndIndices*>(name, vOsAndIndices));
	}

	VOsAndIndices* generateForCube()
	{
		// VAO для куба:
		unsigned cubeVAO, cubeVBO, cubeEBO;
		
		std::vector<float> cubeVertexData = StaticFigures::getCubeVertexesWithNormalsAndUV();
		std::vector<unsigned> cubeIndicesData = StaticFigures::getCubeIndices();
		
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glGenBuffers(1, &cubeEBO);
		
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertexData.size(), cubeVertexData.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * cubeIndicesData.size(), cubeIndicesData.data(), GL_STATIC_DRAW);
		
		// layout (location = 0) in vec3 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
		// layout (location = 1) in vec3 inNormal;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
		// layout (location = 2) in vec2 inTexCoords;
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
		
		glBindVertexArray(0);
		
		VOsAndIndices *generatedVOs = new VOsAndIndices{
			cubeVAO,
			cubeVBO,
			cubeEBO,
			cubeIndicesData
		};
		
		addInLoaded("Cube", generatedVOs);
		
		return generatedVOs;
	}

	VOsAndIndices* generateFor2DQuad()
	{
		// Создаём VAO для 2D quad-а:
		unsigned screenQuadVAO, screenQuadVBO, screenQuadEBO;
		std::vector<float> screenQuadVertexData = StaticFigures::getQuadVertexesWithUV();
		std::vector<unsigned> quadIndicesData = StaticFigures::getQuadIndices();
		
		glGenVertexArrays(1, &screenQuadVAO);
		glGenBuffers(1, &screenQuadVBO);
		glGenBuffers(1, &screenQuadEBO);
		
		glBindVertexArray(screenQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * screenQuadVertexData.size(), screenQuadVertexData.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * quadIndicesData.size(), quadIndicesData.data(), GL_STATIC_DRAW);
		
		// layout (location = 0) in vec2 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		// layout (location = 1) in vec2 inTexCoords;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
		
		glBindVertexArray(0);
		
		VOsAndIndices *generatedVOs = new VOsAndIndices{
			screenQuadVAO,
			screenQuadVBO,
			screenQuadEBO,
			quadIndicesData
		};
		
		addInLoaded("2DQuad", generatedVOs);
		
		return generatedVOs;
	}

	VOsAndIndices* generateForGroundQuad()
	{
		// Создаём VAO для 3D quad-а:
		unsigned worldQuadVAO, worldQuadVBO, worldQuadEBO;
		std::vector<float> worldQuadVertexData = StaticFigures::getWorldQuadVertexesWithNormalsAndUV();
		std::vector<unsigned> quadIndices = StaticFigures::getQuadIndices();

		glGenVertexArrays(1, &worldQuadVAO);
		glGenBuffers(1, &worldQuadVBO);
		glGenBuffers(1, &worldQuadEBO);

		glBindVertexArray(worldQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, worldQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * worldQuadVertexData.size(), worldQuadVertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, worldQuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * quadIndices.size(), quadIndices.data(), GL_STATIC_DRAW);

		// layout (location = 0) in vec3 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
		// layout (location = 1) in vec3 inNormal;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
		// layout (location = 2) in vec2 inTextureCoord;
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

		glBindVertexArray(0);

		VOsAndIndices *generatedVOs = new VOsAndIndices{
			worldQuadVAO,
			worldQuadVBO,
			worldQuadEBO,
			quadIndices
		};
		
		addInLoaded("3DQuad", generatedVOs);
		
		return generatedVOs;
	}

	VOsAndIndices* generateForSkybox()
	{
		// Создаём VAO для skybox-а:
		std::vector<float> skyboxVertexesData = StaticFigures::getCubeVertexes();
		std::vector<unsigned> cubeIndicesData = StaticFigures::getCubeIndices();
		
		unsigned skyboxVAO, skyboxVBO, skyboxEBO;

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);

		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertexesData.size(), skyboxVertexesData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * cubeIndicesData.size(), cubeIndicesData.data(), GL_STATIC_DRAW);

		// layout (location = 0) in vec3 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

		glBindVertexArray(0);
		
		VOsAndIndices *generatedVOs = new VOsAndIndices{
			skyboxVAO,
			skyboxVBO,
			skyboxEBO,
			cubeIndicesData
		};
		
		addInLoaded("Skybox", generatedVOs);
		
		return generatedVOs;
	}

	VOsAndIndices* generateForFont()
	{
		std::vector<unsigned> fontIndicesData = StaticFigures::getQuadIndices();
		
		// VAO для отрисовки шрифта с динамическим VBO
		unsigned fontVAO, fontVBO, fontEBO;
		
		glGenVertexArrays(1, &fontVAO);
		glGenBuffers(1, &fontVBO);
		glGenBuffers(1, &fontEBO);
		
		glBindVertexArray(fontVAO);
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
		// Временно пустой VBO (4-ре вертекса по 4-ре float):
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fontEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * fontIndicesData.size(), fontIndicesData.data(), GL_STATIC_DRAW);
		
		// layout (location = 0) in vec2 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		// layout (location = 1) in vec2 inTextureCoord;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		VOsAndIndices *generatedVOs = new VOsAndIndices{
			fontVAO,
			fontVBO,
			fontEBO,
			fontIndicesData
		};
		
		addInLoaded("Font", generatedVOs);
		
		return generatedVOs;
	}

public:

	static VAOBuilder* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new VAOBuilder();
		}
		return instance;
	}
	
	VOsAndIndices* getByName(const std::string& name) const
	{
		const auto it = loadedVOs->find(name);
		if(it != loadedVOs->end())
		{
			return it->second;
		}
		return nullptr;
	}
	
	VOsAndIndices* getCube()
	{
		VOsAndIndices *returnValue = getByName("Cube");
		if (returnValue == nullptr)
		{
			returnValue = generateForCube();
		}
		return returnValue;
	}
	
	VOsAndIndices* get2DQuad()
	{
		VOsAndIndices *returnValue = getByName("2DQuad");
		if (returnValue == nullptr)
		{
			returnValue = generateFor2DQuad();
		}
		return returnValue;
	}
	
	VOsAndIndices* getGroundQuad()
	{
		VOsAndIndices *returnValue = getByName("3DQuad");
		if (returnValue == nullptr)
		{
			returnValue = generateForGroundQuad();
		}
		return returnValue;
	}

	VOsAndIndices* getSkybox()
	{
		VOsAndIndices *returnValue = getByName("Skybox");
		if (returnValue == nullptr)
		{
			returnValue = generateForSkybox();
		}
		return returnValue;
	}

	VOsAndIndices* getFont()
	{
		VOsAndIndices *returnValue = getByName("Font");
		if (returnValue == nullptr)
		{
			returnValue = generateForFont();
		}
		return returnValue;
	}
};
