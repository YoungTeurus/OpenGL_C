#pragma once
#include "Scene.h"
#include "../camera/Camera.h"
#include "../light/PointLight.h"
#include "../model/ModelLoader.h"
#include "../model/StaticFigures.h"
#include "../shader/ShaderLoader.h"
#include "../utility/FilePaths.h"
#include "../window/Clock.h"
#include "../window/Window.h"
#include "WorldObjects/Tank.h"

class Game
{
private:
	Camera camera = Camera(
		0.0f,
		glm::vec3(0.0f, 0.0f, 0.0f));
	
	Clock clock;

	Scene mainScene;

	ShaderLoader shaderLoader;
	ModelLoader modelLoader;

	void loadShaders()
	{
		std::string pathToShaderFolder = FilePaths::getPathToShaderFolderWithTrailingSplitter();
		shaderLoader.loadShader("assimpModelWithLightsAndExplosionShader", "backpack_mixLightWithExplosion", pathToShaderFolder, true);
		shaderLoader.loadShader("screenRenderQuadWithHDRShader", "screenRenderQuadShaderWithHDR", pathToShaderFolder);
		shaderLoader.loadShader("skyboxShader", "skybox", pathToShaderFolder);
	}

	void loadModels()
	{
		modelLoader.loadModel("tankBase", FilePaths::getPathToModel("tank/base.obj"), true);
		modelLoader.loadModel("tankTurret", FilePaths::getPathToModel("tank/turret.obj"), true);
	}

	// unsigned cubeMapTextureId;
	// 
	// static unsigned loadCubeMapFromPathsAndGetTextureId(const std::vector<string>& textureFileNames, const std::string& pathToTexturesFolder)
	// {
	// 	unsigned textureID;
	// 
	// 	glGenTextures(1, &textureID);
	// 	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	// 
	// 	int width, height, nrChannels;
	// 	unsigned char* data;
	// 	for(unsigned i = 0; i < textureFileNames.size(); i++)
	// 	{
	// 		string pathToTexture = pathToTexturesFolder + textureFileNames[i];
	// 		data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);
	// 		if (data){
	// 			glTexImage2D(
	// 				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	// 				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	// 			);
	// 		} else {
	// 			std::cout << "Cubemap tex failed to load at path: " << textureFileNames[i] << std::endl;
	// 		}
	// 		stbi_image_free(data);
	// 	}
	// 
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
	// 	
	// 	return textureID;
	// }
	
	// void loadSkyboxTexture()
	// {
	// 	string pathToTexturesFolder = FilePaths::getPathToTexturesFolderWithTrailingSplitter();
	// 	std::vector<string> cubeMapFacesTexturePaths = {
	// 	R"(skybox/right.jpg)",
	// 	R"(skybox/left.jpg)",
	// 	R"(skybox/top.jpg)",
	// 	R"(skybox/bottom.jpg)",
	// 	R"(skybox/front.jpg)",
	// 	R"(skybox/back.jpg)",
	// 	};
	// 
	// 	cubeMapTextureId = loadCubeMapFromPathsAndGetTextureId(cubeMapFacesTexturePaths, pathToTexturesFolder);
	// }

	bool isFrameBufferReady = false;
	unsigned frameBuffer;
	unsigned frameBufferColorTextureID;

	unsigned screenQuadVAO;
	vector<unsigned> screenQuadIndicesData = StaticFigures::getQuadIndices();
	
	void prepareScreenQuadVAO()
	{
		unsigned screenQuadVBO, screenQuadEBO;
		vector<float> screenQuadVertexData = StaticFigures::getQuadVertexesWithUV();

		glGenVertexArrays(1, &screenQuadVAO);
		glGenBuffers(1, &screenQuadVBO);
		glGenBuffers(1, &screenQuadEBO);

		glBindVertexArray(screenQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * screenQuadVertexData.size(), screenQuadVertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * screenQuadIndicesData.size(), screenQuadIndicesData.data(), GL_STATIC_DRAW);

		// layout (location = 0) in vec2 inFragPosition;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		// layout (location = 1) in vec2 inTexCoords;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
	}

	unsigned skyboxVAO;
	vector<unsigned> cubeIndicesData = StaticFigures::getCubeIndices();

	void prepareSkyboxVAO()
	{
		// Создаём VAO для skybox-а:
		std::vector<float> skyboxVertexesData = StaticFigures::getCubeVertexes();
		
		unsigned skyboxVBO, skyboxEBO;

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
	}

	Tank *mainTank;
	
	void constructMainScene()
	{
		mainScene.backgroundColor = glm::vec4{0.25f, 0.25f, 0.25f, 1.0f};

		mainTank = new Tank(
				"Main Tank",
				shaderLoader.getShader("assimpModelWithLightsAndExplosionShader"),
				modelLoader.getModel("tankBase"),
				modelLoader.getModel("tankTurret"),
				SceneObjectTransformations(glm::vec3(0.0f, 0.0f, -5.0f))
			);

		mainScene.objects.push_back(mainTank);

		mainScene.lights.push_back(
			new PointLight(
				glm::vec3(0.0f),
				glm::vec3(0.1f, 0.0f, 0.0f),
				glm::vec3(0.1f, 0.0f, 0.0f),
				1.0f, 0.001f, 0.0009f,
				glm::vec3(0.0f, 3.0f, -5.0f),
				"Dim red lamp"
			)
		);
		
		mainScene.lights.push_back(
			new PointLight(
				glm::vec3(0.0f),
				glm::vec3(0.0f, 0.0f, 0.2f),
				glm::vec3(0.0f, 0.0f, 0.2f),
				1.0f, 0.001f, 0.0009f,
				glm::vec3(0.0f, 3.0f, -3.0f),
				"Dim green lamp"
			)
		);
		
		mainScene.lights.push_back(
			new PointLight(
				glm::vec3(0.0f),
				glm::vec3(0.0f, 0.1f, 0.0f),
				glm::vec3(0.0f, 0.1f, 0.0f),
				1.0f, 0.001f, 0.0009f,
				glm::vec3(0.0f, 3.0f, -1.0f),
				"Dim blue lamp"
			)
		);
	}

	void updateSceneObjects()
	{
		
	}

	void renderSceneObjects()
	{
		if (!isFrameBufferReady)
		{
			throw new exception("Game::renderSceneObjects : framebuffer wasn't provided.");
		}
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix();

		glm::mat4 pv = projection * view;

		// Отрисовка в framebuffer:
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glEnable(GL_DEPTH_TEST);
		
		glClearColor(mainScene.backgroundColor.r, mainScene.backgroundColor.g, mainScene.backgroundColor.b, mainScene.backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader *assimpModelWithLightsAndExplosionShader = shaderLoader.getShader("assimpModelWithLightsAndExplosionShader");

		assimpModelWithLightsAndExplosionShader->use();
		// Установка света:
		int activeLights = 0;
		for (int i = 0; i < mainScene.lights.size(); i++)
		{
			activeLights += mainScene.lights[i]->useAndReturnSuccess(assimpModelWithLightsAndExplosionShader, activeLights);
		}
		assimpModelWithLightsAndExplosionShader->setInt("lightsCount", activeLights);

		mainTank->baseTransformations.position = glm::vec3(glm::cos(clock.getCurrentTimestamp()) * 2.5f, 0.0f, glm::cos(clock.getCurrentTimestamp()) * 2.5f);
		mainTank->baseTransformations.rotationAngleDegrees = glm::cos(clock.getCurrentTimestamp()) * 30.0f;
		mainTank->turretTransformations.rotationAngleDegrees = glm::cos(clock.getCurrentTimestamp()) * 90.0f;
		
		// // Отрисовка skybox:
		// Shader *skyboxShader = shaderLoader.getShader("skyboxShader");
		// 
		// glDepthFunc(GL_LEQUAL);
		// skyboxShader->use();
		// 
		// // Избавляемся от перемещения камеры, сохраняя поворот
		// glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
		// 
		// skyboxShader->setFloatMat4("projection", projection);
		// skyboxShader->setFloatMat4("view", skyboxView);
		// 
		// glBindVertexArray(skyboxVAO);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		// glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		// glDepthMask(GL_TRUE);  // Последующие элементы влияют на Depth
		// 
		// glDepthFunc(GL_LESS);
		// // Конец отрисовки skybox-а
		
		// Конец отрисовки в framebuffer.

		// Отрисовка в стандартный framebuffer quad-а, занимающего весь экран:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);

		Shader *screenRenderQuadWithHDRShader = shaderLoader.getShader("screenRenderQuadWithHDRShader");
		
		screenRenderQuadWithHDRShader->use();
		// screenRenderQuadWithHDRShader->setFloat("exposure", globalExposure);
		glBindVertexArray(screenQuadVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferColorTextureID);
		glDrawElements(GL_TRIANGLES, screenQuadIndicesData.size(), GL_UNSIGNED_INT, 0);
		// Конец отрисовки в стандартный framebuffer.
	}

public:
	Game()
	{
		loadShaders();
		loadModels();
		// loadSkyboxTexture();
		prepareScreenQuadVAO();
		prepareSkyboxVAO();
		constructMainScene();
	}
	
	void mainCycleTick()
	{
		clock.updateTime();
		updateSceneObjects();
		renderSceneObjects();
	}

	void setFrameBuffer(unsigned frameBuffer, unsigned frameBufferColorTextureID)
	{
		this->frameBuffer = frameBuffer;
		this->frameBufferColorTextureID = frameBufferColorTextureID;
		this->isFrameBufferReady = true;
	}

	void setCameraAspectRatio(float ratio)
	{
		camera.aspectRatio = ratio;
	}

	void setCameraPosition(const glm::vec3& pos)
	{
		camera.position = pos;
		camera.updateCameraVectors();
	}

	void setCameraRotation(float pitch, float yaw)
	{
		camera.pitch = pitch;
		camera.yaw = yaw;
		camera.updateCameraVectors();
	}
};
