#pragma once
#include "VOsAndIndices.h"

class VAOBuilder
{
private:
	
	static std::map<std::string, VOsAndIndices*>* loadedVOs;

	static void addInLoaded(const std::string& name, VOsAndIndices* vOsAndIndices)
	{
		loadedVOs->insert(std::pair<std::string, VOsAndIndices*>(name, vOsAndIndices));
	}

	static VOsAndIndices* generateForCube()
	{
		// VAO ��� ����:
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

	static VOsAndIndices* generateFor2DQuad()
	{
		// ������ VAO ��� 2D quad-�:
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

	static VOsAndIndices* generateFor3DQuad()
	{
		// ������ VAO ��� 3D quad-�:
		unsigned worldQuadVAO, worldQuadVBO, worldQuadEBO;
		vector<float> worldQuadVertexData = StaticFigures::getWorldQuadVertexesWithNormalsAndUV();
		vector<unsigned> quadIndices = StaticFigures::getQuadIndices();

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

	static VOsAndIndices* generateForSkybox()
	{
		// ������ VAO ��� skybox-�:
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

public:
	
	static VOsAndIndices* getByName(const std::string& name)
	{
		const auto it = loadedVOs->find(name);
		if(it != loadedVOs->end())
		{
			return it->second;
		}
		return nullptr;
	}
	
	static VOsAndIndices* getCube()
	{
		VOsAndIndices *returnValue = getByName("Cube");
		if (returnValue == nullptr)
		{
			returnValue = generateForCube();
		}
		return returnValue;
	}
	
	static VOsAndIndices* get2DQuad()
	{
		VOsAndIndices *returnValue = getByName("2DQuad");
		if (returnValue == nullptr)
		{
			returnValue = generateFor2DQuad();
		}
		return returnValue;
	}
	
	static VOsAndIndices* get3DQuad()
	{
		VOsAndIndices *returnValue = getByName("3DQuad");
		if (returnValue == nullptr)
		{
			returnValue = generateFor3DQuad();
		}
		return returnValue;
	}

	static VOsAndIndices* getSkybox()
	{
		VOsAndIndices *returnValue = getByName("Skybox");
		if (returnValue == nullptr)
		{
			returnValue = generateForSkybox();
		}
		return returnValue;
	}
};

std::map<std::string, VOsAndIndices*>* VAOBuilder::loadedVOs = new std::map<std::string, VOsAndIndices*>();