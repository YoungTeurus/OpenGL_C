// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "light/DirectionalLight.h"
#include "world/LightCube.h"
#include "shader/Shader.h"
#include "camera/Camera.h"
#include "model/Model.h"
#include "model/ModelTransformations.h"
#include "model/VAOBuilder.h"
#include "model/VOsAndIndices.h"
#include "renderer/Renderer.h"
#include "shader/ShaderLoader.h"
#include "textures/Texture.h"
#include "textures/CubeMap.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

bool firstMouse = true;

Renderer* renderer = Renderer::getInstance();
Camera mainCamera = renderer->getMainCamera();

bool wireframeMode = false;

float globalExposure = 1.0f;
float globalGamma = 2.2f;

float globalExplosionForce = 0.0f;
float globalTimeSinceExplosion = 0.0f;

void setPolygoneDrawMode() {
	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void changeSpotlightState(SpotLight *spotLight)
{
	spotLight->setIsActive(!spotLight->getIsActive());
}

SpotLight *flashlight;

void changeFlashlightState()
{
	changeSpotlightState(flashlight);
}

struct Color
{
	float r, g, b, a;
};

Color background = {0.25f, 0.25f, 0.25f, 1.f};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

// Callback функция, вызываемая при изменении размеров окна.
void OnResize(GLFWwindow* window, int width, int height)
{
	mainCamera.aspectRatio = (float)width / height;
	glViewport(0, 0, width, height);
}

// Работа с устройствами ввода
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}

	// Изменение цвета фона (для отладки):
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
		background = {1.f, 0.f, 0.f, 1.f};
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		background = {0.f, 1.f, 0.f, 1.f};
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
		background = {0.f, 0.f, 1.f, 1.f};
	}

	// Перемещение камеры:
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::STRAFE_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::STRAFE_RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		globalExposure = min( globalExposure + 1.0f * deltaTime, 10.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		globalExposure = max( globalExposure - 1.0f * deltaTime, 0.01f );
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		globalGamma = min( globalGamma + 0.5f * deltaTime, 5.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		globalGamma = max( globalGamma - 0.5f * deltaTime, 0.01f );
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		globalExplosionForce = min( globalExplosionForce + 1.f * deltaTime, 20.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		globalExplosionForce = max( globalExplosionForce - 1.f * deltaTime, 0.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		globalTimeSinceExplosion = min( globalTimeSinceExplosion + 0.5f * deltaTime, 10.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		globalTimeSinceExplosion = max( globalTimeSinceExplosion - 0.5f * deltaTime, 0.0f );
	}
	
	// Поднятие-спуск камеры:
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::DOWN, deltaTime);
	}
}

void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_M:
			wireframeMode = !wireframeMode;
			setPolygoneDrawMode();
			break;
		case GLFW_KEY_0:
			globalExplosionForce = 0.0f;
			globalTimeSinceExplosion = 0.0f;
			break;
		case GLFW_KEY_F:
			changeFlashlightState();
			break;
		default:
			break;
		}
	}
}

// Обработка движения колёсика мыши
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	mainCamera.handleMouseScroll((float)yOffset);
}

// Обработка движения мыши
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	// Обработка первого движения камеры:
	if (firstMouse) {
		lastCursorX = (float)xpos;
		lastCursorY = (float)ypos;
		firstMouse = false;
	}

	// Подсчитываем смещение курсора относительно предыдущего кадра:
	float dx = (float)xpos - lastCursorX,
		  dy = lastCursorY - (float)ypos;  // Отражаем Y, так как в OpenGl ось Y идёт вверх, а в окнах - вниз
	lastCursorX = (float)xpos; lastCursorY = (float)ypos;

	mainCamera.handleMouseMovement(dx, dy);
}

int main()
{
	using namespace std;
	
#pragma region GLFW INIT
	// Настройка glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	int windowWidth = windowInitialWidth, windowHeight = windowInitialHeight;

	GLFWwindow* win = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Window",
	                                   NULL, NULL);

	if (win == NULL)
	{
		cout << "Error while creating the window.";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

	// Получение функций видеокарты
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		cout << "Error while loading GLAD.";
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Захватываем курсор
	glfwSetCursorPosCallback(win, mouseCallback);  // Функция, вызываемая при перемещении курсора
	glfwSetScrollCallback(win, scrollCallback);
	glfwSetKeyCallback(win, onKeyAction);
	setPolygoneDrawMode();

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_MULTISAMPLE);

	// TODO: вернуть отбрасывание граней.
	// glEnable(GL_CULL_FACE);
	// Конец настройки glfw
#pragma endregion

	mainCamera.setAspectRatio((float)windowInitialWidth / windowInitialHeight);
	mainCamera.setPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	// Загрузка внешних данных:
	std::string pathToShaderFolder = FilePaths::getPathToShaderFolderWithTrailingSplitter();
	Shader* assimpModelWithLightsAndExplosionShader = ShaderLoader::getInstance()->load("backpack_mixLightWithExplosion", pathToShaderFolder, true);
	Shader* screenRenderQuadShaderWithBlur = ShaderLoader::getInstance()->load("screenRenderQuadShaderWithBlur", pathToShaderFolder);
	Shader* screenRenderQuadWithHDRShader = ShaderLoader::getInstance()->load("screenRenderQuadShaderWithHDR", pathToShaderFolder);
	Shader* skyboxShader = ShaderLoader::getInstance()->load("skybox", pathToShaderFolder);
	Shader* groundQuad_mixLight = ShaderLoader::getInstance()->load("groundQuad_mixLight", pathToShaderFolder);
	
	Model tankBase(FilePaths::getPathToModel("tank/base.obj"), true);
	Model tankTurret(FilePaths::getPathToModel("tank/turret.obj"), true);

	CubeMap cubeMapTexture;
	cubeMapTexture.loadFromFile("sky.jpg", FilePaths::getPathToTexturesFolderWithTrailingSplitter() + "skyboxNew");

	Texture groundTexture;
	groundTexture.loadFromFile("grass.png", FilePaths::getPathToTexturesFolder());
	Texture groundSpecularTexture;
	groundSpecularTexture.loadFromFile("grass_specular.png", FilePaths::getPathToTexturesFolder());

	
	// Подготовка источников освещения:
	vector<PositionedLight*> positionedLights;
	vector<LightCube*> lightCubes;

	PointLight *pointLight = new PointLight(
			glm::vec3(0.1f),
			glm::vec3(20.0f, 20.0f, 5.0f),
			glm::vec3(20.0f, 20.0f, 5.0f),
			1.0f, 0.01f, 0.009f,
			glm::vec3(0.0f, 10.0f, -10.0f),
			"VERY bright white lamp"
		);

	renderer->addLight(pointLight);
	positionedLights.push_back(pointLight);
	lightCubes.push_back( new LightCube(pointLight) );
	
	// lights.push_back(
	// 	new PointLight(
	// 		glm::vec3(0.0f),
	// 		glm::vec3(0.1f, 0.0f, 0.0f),
	// 		glm::vec3(0.1f, 0.0f, 0.0f),
	// 		1.0f, 0.001f, 0.0009f,
	// 		glm::vec3(0.0f, 3.0f, -5.0f),
	// 		"Dim red lamp"
	// 	)
	// );
	
	// lights.push_back(
	// 	new PointLight(
	// 		glm::vec3(0.0f),
	// 		glm::vec3(0.0f, 0.0f, 0.2f),
	// 		glm::vec3(0.0f, 0.0f, 0.2f),
	// 		1.0f, 0.001f, 0.0009f,
	// 		glm::vec3(0.0f, 3.0f, -3.0f),
	// 		"Dim green lamp"
	// 	)
	// );
	
	// lights.push_back(
	// 	new PointLight(
	// 		glm::vec3(0.0f),
	// 		glm::vec3(0.0f, 0.1f, 0.0f),
	// 		glm::vec3(0.0f, 0.1f, 0.0f),
	// 		1.0f, 0.001f, 0.0009f,
	// 		glm::vec3(0.0f, 3.0f, -1.0f),
	// 		"Dim blue lamp"
	// 	)
	// );
	
	flashlight = new SpotLight(
		glm::radians(10.f), glm::radians(20.f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(3.7f, 3.7f, 3.6f),
		glm::vec3(8.8f, 8.8f, 8.7f),
		1.0f, 0.05f, 0.009f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"FlashLight"
	);
	renderer->addLight(flashlight);
	positionedLights.push_back(flashlight);

#pragma region Инициализация Framebuffer-а и разных VAO

	// Рендеринг в текстуру:
	unsigned frameBuffer;
	// Создание фреймбуффера:
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Создание текстуры, на котороую будут выводится рендер frameBuffer-а.
	unsigned frameBufferColorTexturesID[2];
	glGenTextures(2, frameBufferColorTexturesID);
	for(unsigned i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, frameBufferColorTexturesID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Присоединяем созданную текстуру к framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBufferColorTexturesID[i], 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);
	
	// Создание render-буфера для depth и stencil тестов:
	// Render-буфер работает в режиме write-only.
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Присоединяем созданный buffer к framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// Переключаемся на стандартный framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Создание framebuffer-ов для размытия Гаусса:
	unsigned pingpongFrameBuffers[2];
	unsigned pingpongTextureIds[2];
	glGenFramebuffers(2, pingpongFrameBuffers);
	glGenTextures(2, pingpongTextureIds);
	for(unsigned i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffers[i]);
	    glBindTexture(GL_TEXTURE_2D, pingpongTextureIds[i]);
	    glTexImage2D(
	        GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
	    );
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glFramebufferTexture2D(
	        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextureIds[i], 0
	    );
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	VOsAndIndices *screenQuadVOsAndIndices = VAOBuilder::getInstance()->get2DQuad();
	VOsAndIndices *worldQuadVOsAndIndices = VAOBuilder::getInstance()->getGroundQuad();
	VOsAndIndices *skyboxVOsAndIndices = VAOBuilder::getInstance()->getSkybox();
	
#pragma endregion 
	

	ModelTransformations tankBaseTransformations = {
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};
	ModelTransformations tankTurretTransformations = {
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};
	ModelTransformations tankBaseTransformations2 = {
		glm::vec3(-5.0f, 0.0f, -15.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};
	ModelTransformations tankTurretTransformations2 = {
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};
	ModelTransformations tankBaseTransformations3 = {
		glm::vec3(-15.0f, 0.0f, -30.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};
	ModelTransformations tankTurretTransformations3 = {
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f)
	};

	ModelTransformations groundTransformation = {
		glm::vec3(0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		90.0f,
		glm::vec3(100.0f)
	};

	while (!glfwWindowShouldClose(win))
	{
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		processInput(win);

		// Изменение положения источников света:
		flashlight->setPosition(mainCamera.position
			- mainCamera.up * 0.3f
		);
		flashlight->setDirection(mainCamera.front);

		// Вращение камеры:
		glm::mat4 view = mainCamera.getViewMatrix();

		// Поправка на FOV камеры:
		glm::mat4 projection = mainCamera.getProjectionMatrix();

		renderer->setViewAndProjection(view, projection);

		// Отрисовка в framebuffer:
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glEnable(GL_DEPTH_TEST);
		
		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Отрисовка модели:
		tankBaseTransformations.position = glm::vec3(glm::cos(currentTime) * 2.5f, 0.0f, glm::cos(currentTime) * 2.5f);
		tankBaseTransformations.rotationAngleDegrees = glm::cos(currentTime) * 30.0f;
		glm::mat4 tankBaseModel = tankBaseTransformations.createModelMatrixWithTransformations();
		
		assimpModelWithLightsAndExplosionShader->use();
		assimpModelWithLightsAndExplosionShader->setFloat("uExplosionMagnitude", globalExplosionForce);
		assimpModelWithLightsAndExplosionShader->setFloat("uTimeSinceExplosion", globalTimeSinceExplosion);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("projectionAndView", renderer->getPV());
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankBaseModel);
		assimpModelWithLightsAndExplosionShader->setFloat("shininess", 64.0f);
		assimpModelWithLightsAndExplosionShader->setFloatVec3("viewPos", mainCamera.position);
		
		int activeLights = 0;
		for(auto *light : renderer->getLights())
		{
			activeLights += light->useAndReturnSuccess(assimpModelWithLightsAndExplosionShader, activeLights);
		}
		
		assimpModelWithLightsAndExplosionShader->setInt("lightsCount", activeLights);
		
		tankBase.draw(*assimpModelWithLightsAndExplosionShader);
		
		tankTurretTransformations.rotationAngleDegrees = glm::cos(currentTime) * 90.0f;
		glm::mat4 tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = tankTurretTransformations.getModelWithAppliedTransformations(tankTurretModel);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankTurretModel);
		tankTurret.draw(*assimpModelWithLightsAndExplosionShader);

		// Копии:
		tankBaseModel = tankBaseTransformations2.createModelMatrixWithTransformations();
		assimpModelWithLightsAndExplosionShader->setFloat("uExplosionMagnitude", 0.0f);
		assimpModelWithLightsAndExplosionShader->setFloat("uTimeSinceExplosion", 0.0f);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankBaseModel);
		tankBase.draw(*assimpModelWithLightsAndExplosionShader);

		tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = tankTurretTransformations2.getModelWithAppliedTransformations(tankTurretModel);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankTurretModel);
		tankTurret.draw(*assimpModelWithLightsAndExplosionShader);

		tankBaseModel = tankBaseTransformations3.createModelMatrixWithTransformations();
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankBaseModel);
		tankBase.draw(*assimpModelWithLightsAndExplosionShader);

		tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = tankTurretTransformations3.getModelWithAppliedTransformations(tankTurretModel);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankTurretModel);
		tankTurret.draw(*assimpModelWithLightsAndExplosionShader);

		// Отрисовка "светящегося куба".
		for(auto *lightCube : lightCubes)
		{
			lightCube->draw(renderer);
		}
		
		// Отрисовка "земли".
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glm::mat4 groundModel = groundTransformation.createModelMatrixWithTransformations();
		groundQuad_mixLight->use();
		groundQuad_mixLight->setFloatMat4("projectionAndView", renderer->getPV());
		groundQuad_mixLight->setFloatMat4("model", groundModel);
		groundQuad_mixLight->setFloat("shininess", 64.0f);
		groundQuad_mixLight->setFloatVec3("viewPos", mainCamera.position);

		int activeLights0 = 0;
		for(auto *light : renderer->getLights())
		{
			activeLights0 += light->useAndReturnSuccess(groundQuad_mixLight, activeLights0);
		}
		groundQuad_mixLight->setInt("lightsCount", activeLights0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundTexture.getId());
		groundQuad_mixLight->setInt("texture_diffuse", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, groundSpecularTexture.getId());
		groundQuad_mixLight->setInt("texture_specular", 1);

		glBindVertexArray(worldQuadVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, worldQuadVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);

		// Отрисовка skybox:
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();
		
		// Избавляемся от перемещения камеры, сохраняя поворот
		glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
		
		skyboxShader->setFloatMat4("projection", projection);
		skyboxShader->setFloatMat4("view", skyboxView);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.getId());
		glBindVertexArray(skyboxVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, skyboxVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
		glDepthMask(GL_TRUE);  // Последующие элементы влияют на Depth
		
		glDepthFunc(GL_LESS);
		// Конец отрисовки skybox-а
		// Конец отрисовки в framebuffer.

		// Отрисовка в frambuffer для размытия:
		bool horizontal = true, firstIteration = true;
		unsigned amount = 5;
		screenRenderQuadShaderWithBlur->use();
		for(unsigned i = 0; i < amount * 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffers[horizontal]);
			screenRenderQuadShaderWithBlur->setInt("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, firstIteration ? frameBufferColorTexturesID[1] : pingpongTextureIds[!horizontal]);

			glBindVertexArray(screenQuadVOsAndIndices->vao);
			glDrawElements(GL_TRIANGLES, screenQuadVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
			
			horizontal = !horizontal;
			if (firstIteration)
			{
				firstIteration = false;
			}
		}
		
		// Отрисовка в стандартный framebuffer quad-а, занимающего весь экран:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
		
		screenRenderQuadWithHDRShader->use();
		screenRenderQuadWithHDRShader->setFloat("exposure", globalExposure);
		screenRenderQuadWithHDRShader->setFloat("gamma", globalGamma);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferColorTexturesID[0]);
		screenRenderQuadWithHDRShader->setInt("hdrBuffer", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongTextureIds[0]);
		screenRenderQuadWithHDRShader->setInt("blurBuffer", 1);
		glBindVertexArray(screenQuadVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, screenQuadVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
		// Конец отрисовки в стандартный framebuffer.

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
