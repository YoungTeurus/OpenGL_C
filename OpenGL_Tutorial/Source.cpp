// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>


#include "Cube.h"
#include "PointLight.h"
#include "classes/Shader.h"
#include "classes/Camera.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "SpotLight.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

bool firstMouse = true;

Camera mainCamera = Camera((float)windowInitialWidth / windowInitialHeight, glm::vec3(0.0f, 0.0f, 0.0f));

bool wireframeMode = false;

void setPolygoneDrawMode() {
	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

struct Color
{
	float r, g, b, a;
};

Color background = {0.25f, 0.25f, 0.25f, 1.f};

struct ModelTransformations
{
	glm::vec3 position;
	glm::vec3 rotationVector;
	float rotationAngleDegrees;
	glm::vec3 scale;

	glm::mat4 createModelMatrixWithTransformations() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		return getModelWithAppliedTransformations(model);
	}

	glm::mat4 getModelWithAppliedTransformations(glm::mat4 model) const{
		glm::mat4 transformedModel = model;

		transformedModel = glm::translate(transformedModel, position);
		transformedModel = glm::rotate(transformedModel, glm::radians(rotationAngleDegrees), rotationVector);
		transformedModel = glm::scale(transformedModel, scale);

		return transformedModel;
	}
};

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

	// TODO: сделать поворот камеры вокруг оси Z
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::ROLL_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::ROLL_RIGHT, deltaTime);
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

unsigned loadCubeMapFromPathsAndGetTextureId(std::vector<string> pathsToTextures)
{
	unsigned textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for(unsigned i = 0; i < pathsToTextures.size(); i++)
	{
		data = stbi_load(pathsToTextures[i].c_str(), &width, &height, &nrChannels, 0);
		if (data){
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		} else {
			std::cout << "Cubemap tex failed to load at path: " << pathsToTextures[i] << std::endl;
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
	
	return textureID;
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

	// Загрузка внешних данных:
	Shader* assimpModelWithLightsShader = new Shader("backpack_mixLightWithExplosion", true);
	Shader* lightCubeShader = new Shader("lightCubeWithExplosion", true);
	Shader* singleColorShader = new Shader("shaderSingleColor");
	Shader* screenRenderQuadShader = new Shader("screenRenderQuadShader");
	Shader* skyboxShader = new Shader("skybox");
	
	Model tankBase("models/tank/base.obj", true);
	Model tankTurret("models/tank/turret.obj", true);

	// Создание Cubemap-ы:
	std::vector<string> cubeMapFacesTexturePaths = {
		R"(./textures/skybox/right.jpg)",
		R"(./textures/skybox/left.jpg)",
		R"(./textures/skybox/top.jpg)",
		R"(./textures/skybox/bottom.jpg)",
		R"(./textures/skybox/front.jpg)",
		R"(./textures/skybox/back.jpg)",
	};

	unsigned cubeMapTextureId = loadCubeMapFromPathsAndGetTextureId(cubeMapFacesTexturePaths);

	// Подготовка источников освещения:
	vector<BaseLight*> lights;

	// PointLight* brightLamp = new PointLight(
	// 	glm::vec3(0.15f, 0.15f, 0.15f),
	// 	glm::vec3(0.75f, 0.75f, 0.75f),
	// 	glm::vec3(1.0f, 1.0f, 1.0f),
	// 	1.0f, 0.001f, 0.0009f,
	// 	glm::vec3(5.0f, 5.0f, 5.0f),
	// 	"BrightLamp"
	// );
	// lights.push_back(brightLamp);
	// 
	// PointLight* redLamp = new PointLight(
	// 	glm::vec3(0.1f, 0.1f, 0.1f),
	// 	glm::vec3(1.0f, 0.2f, 0.2f),
	// 	glm::vec3(1.0f, 0.2f, 0.2f),
	// 	1.0f, 0.1f, 0.09f,
	// 	glm::vec3(0.0f, 0.0f, 0.0f),
	// 	"RedLamp"
	// );
	// lights.push_back(redLamp);
	// 
	// PointLight* blueLamp = new PointLight(
	// 	glm::vec3(0.1f, 0.1f, 0.1f),
	// 	glm::vec3(0.2f, 0.2f, 1.0f),
	// 	glm::vec3(0.2f, 0.2f, 1.0f),
	// 	1.0f, 0.1f, 0.09f,
	// 	glm::vec3(0.0f, 0.0f, 0.0f),
	// 	"BlueLamp"
	// );
	// lights.push_back(blueLamp);
	// 
	// DirectionalLight* sunLight = new DirectionalLight(
	// 	glm::vec3(-1.0f, -1.0f, -1.0f),
	// 	glm::vec3(0.1f, 0.1f, 0.1f),
	// 	glm::vec3(0.5f, 0.5f, 0.5f),
	// 	glm::vec3(0.0f, 0.0f, 0.0f),
	// 	"Sun"
	// );
	// lights.push_back(sunLight);
	
	SpotLight* flashLight = new SpotLight(
		glm::radians(10.f), glm::radians(20.f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.7f, 0.7f, 0.6f),
		glm::vec3(0.8f, 0.8f, 0.7f),
		1.0f, 0.001f, 0.0009f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"FlashLight"
	);
	lights.push_back(flashLight);

	vector<Cube> lightCubes;
	
	for (auto && light : lights)
	{
		PositionedLight* positionedLight;
		if((positionedLight = dynamic_cast<PositionedLight*>(light)) != nullptr)
		{
			lightCubes.emplace_back(
			Cube(positionedLight->getPosition(), glm::vec3(0.25f), glm::vec3(0.0f), positionedLight->getDiffuse())
			);
		}
	}

	// Рендеринг в текстуру:
	unsigned frameBuffer;
	// Создание фреймбуффера:
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Создание текстуры, на котороую будут выводится рендер frameBuffer-а.
	unsigned frameBufferColorTextureID;
	glGenTextures(1, &frameBufferColorTextureID);
	glBindTexture(GL_TEXTURE_2D, frameBufferColorTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Присоединяем созданную текстуру к framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferColorTextureID, 0);

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
	
	
	// Создаём VAO для quad-а:
	unsigned screenQuadVAO, screenQuadVBO, screenQuadEBO;
	vector<float> screenQuadVertexData = StaticFigures::getQuadVertexesWithUV();
	vector<unsigned> screenQuadIndicesData = StaticFigures::getQuadIndices();

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

	unsigned cubeVAO, cubeVBO, cubeEBO;

	vector<float> cubeVertexData = StaticFigures::getCubeVertexesWithNormalsAndUV();
	vector<unsigned> cubeIndicesData = StaticFigures::getCubeIndices();
	
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

	
	// Создаём VAO для skybox-а:
	std::vector<float> skyboxVertexesData = StaticFigures::getCubeVertexes();
	
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
	ModelTransformations lightCubeTransformation = {
		glm::vec3(-5.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(0.5f)
	};

	while (!glfwWindowShouldClose(win))
	{
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		processInput(win);

		// Изменение положения источников света:
		flashLight->setPosition(mainCamera.position - mainCamera.up * 0.3f);
		flashLight->setDirection(mainCamera.front);

		// Вращение камеры:
		glm::mat4 view = mainCamera.getViewMatrix();

		// Поправка на FOV камеры:
		glm::mat4 projection = mainCamera.getProjectionMatrix();

		glm::mat4 pv = projection * view;

		// Отрисовка в framebuffer:
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glEnable(GL_DEPTH_TEST);
		
		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Отрисовка модели:
		tankBaseTransformations.position = glm::vec3(glm::cos(currentTime) * 2.5f, 0.0f, glm::cos(currentTime) * 2.5f);
		tankBaseTransformations.rotationAngleDegrees = glm::cos(currentTime) * 30.0f;
		glm::mat4 tankBaseModel = tankBaseTransformations.createModelMatrixWithTransformations();
		
		assimpModelWithLightsShader->use();
		assimpModelWithLightsShader->setFloat("uExplosionMagnitude", 0.0f);
		assimpModelWithLightsShader->setFloatMat4("perspectiveAndView", pv);
		assimpModelWithLightsShader->setFloatMat4("model", tankBaseModel);
		assimpModelWithLightsShader->setFloat("shininess", 64.0f);
		assimpModelWithLightsShader->setFloatVec3("viewPos", mainCamera.position);
		
		int activeLights = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights += lights[i]->useAndReturnSuccess(assimpModelWithLightsShader, activeLights);
		}
		assimpModelWithLightsShader->setInt("lightsCount", activeLights);
		
		tankBase.draw(*assimpModelWithLightsShader);

		tankTurretTransformations.rotationAngleDegrees = glm::cos(currentTime) * 90.0f;
		glm::mat4 tankTurretModel = glm::mat4(1.0f);
		tankTurretModel *= tankBaseModel;
		tankTurretModel = tankTurretTransformations.getModelWithAppliedTransformations(tankTurretModel);
		assimpModelWithLightsShader->setFloatMat4("model", tankTurretModel);
		tankTurret.draw(*assimpModelWithLightsShader);
		
		// Отрисовываем кубы в stencil буфер

		lightCubeShader->use();
		
		glm::mat4 lightCubeModel = lightCubeTransformation.createModelMatrixWithTransformations();
		lightCubeShader->setFloatMat4("projectionAndView", pv);
		lightCubeShader->setFloatMat4("model", lightCubeModel);
		lightCubeShader->setFloatVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);

		// Отрисовка skybox:
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();

		// Избавляемся от перемещения камеры, сохраняя поворот
		glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

		skyboxShader->setFloatMat4("projection", projection);
		skyboxShader->setFloatMat4("view", skyboxView);

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);  // Последующие элементы влияют на Depth

		glDepthFunc(GL_LESS);
		// Конец отрисовки skybox-а
		// Конец отрисовки в framebuffer.

		// Отрисовка в стандартный framebuffer quad-а, занимающего весь экран:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		
		screenRenderQuadShader->use();
		glBindVertexArray(screenQuadVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferColorTextureID);
		glDrawElements(GL_TRIANGLES, screenQuadIndicesData.size(), GL_UNSIGNED_INT, 0);
		// Конец отрисовки в стандартный framebuffer.

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
