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
#include "shader/Shader.h"
#include "camera/Camera.h"
#include "model/Model.h"
#include "model/StaticFigures.h"
#include "utility/FilePaths.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

bool firstMouse = true;

Camera mainCamera = Camera((float)windowInitialWidth / windowInitialHeight, glm::vec3(0.0f, 5.0f, 0.0f));

bool wireframeMode = false;

float globalExposure = 1.0f;
float globalGamma = 2.2f;

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

unsigned loadCubeMapFromPathsAndGetTextureId(const std::vector<string>& textureFileNames, const std::string& pathToTexturesFolder)
{
	unsigned textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for(unsigned i = 0; i < textureFileNames.size(); i++)
	{
		string pathToTexture = pathToTexturesFolder + textureFileNames[i];
		data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);
		if (data){
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		} else {
			std::cout << "Cubemap tex failed to load at path: " << textureFileNames[i] << std::endl;
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

unsigned loadTextureFromPathAndGetTextureId(const std::string& pathToTexture)
{
	unsigned textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);
	if (data){
		GLenum format;
		switch (nrChannels) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
		default:
			format = GL_RGBA;
		}
		glTexImage2D(
			GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
		);
	} else {
		std::cout << "Texture failed to load at path: " << pathToTexture << std::endl;
	}
	stbi_image_free(data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	std::string pathToShaderFolder = FilePaths::getPathToShaderFolderWithTrailingSplitter();
	Shader* assimpModelWithLightsAndExplosionShader = new Shader("backpack_mixLightWithExplosion", pathToShaderFolder, true);
	// Shader* lightCubeShader = new Shader("lightCube", pathToShaderFolder, true);
	// Shader* lightCubeWithExplosionShader = new Shader("lightCubeWithExplosion", pathToShaderFolder, true);
	// Shader* singleColorShader = new Shader("shaderSingleColor", pathToShaderFolder);
	// Shader* screenRenderQuadShader = new Shader("screenRenderQuadShader", pathToShaderFolder);
	Shader* screenRenderQuadShaderWithBlur = new Shader("screenRenderQuadShaderWithBlur", pathToShaderFolder);
	Shader* screenRenderQuadWithHDRShader = new Shader("screenRenderQuadShaderWithHDR", pathToShaderFolder);
	Shader* skyboxShader = new Shader("skybox", pathToShaderFolder);
	Shader* groundQuad_mixLight = new Shader("groundQuad_mixLight", pathToShaderFolder);
	// Shader* cubeWithLightsShader = new Shader("cube_mixLight", pathToShaderFolder);
	
	Model tankBase(FilePaths::getPathToModel("tank/base.obj"), true);
	Model tankTurret(FilePaths::getPathToModel("tank/turret.obj"), true);

	// Создание Cubemap-ы:
	std::vector<string> cubeMapFacesTexturePaths = {
		R"(skybox/right.jpg)",
		R"(skybox/left.jpg)",
		R"(skybox/top.jpg)",
		R"(skybox/bottom.jpg)",
		R"(skybox/front.jpg)",
		R"(skybox/back.jpg)",
	};

	string pathToTexturesFolder = FilePaths::getPathToTexturesFolderWithTrailingSplitter();
	unsigned cubeMapTextureId = loadCubeMapFromPathsAndGetTextureId(cubeMapFacesTexturePaths, pathToTexturesFolder);

	// unsigned containerTextureID = loadTextureFromPathAndGetTextureId(FilePaths::getPathToTexture("container2.png"));
	// unsigned containerSpecularTextureID = loadTextureFromPathAndGetTextureId(FilePaths::getPathToTexture("container2_specular.png"));
	unsigned groundTextureID = loadTextureFromPathAndGetTextureId(FilePaths::getPathToTexture("grass.png"));
	unsigned groundSpecularTextureID = loadTextureFromPathAndGetTextureId(FilePaths::getPathToTexture("grass_specular.png"));
	
	// Подготовка источников освещения:
	vector<BaseLight*> lights;

	lights.push_back(
		new PointLight(
			glm::vec3(0.1f),
			glm::vec3(20.0f),
			glm::vec3(20.0f),
			1.0f, 0.01f, 0.009f,
			glm::vec3(0.0f, 3.0f, -10.0f),
			"VERY bright white lamp"
		)
	);

	lights.push_back(
		new PointLight(
			glm::vec3(0.0f),
			glm::vec3(0.1f, 0.0f, 0.0f),
			glm::vec3(0.1f, 0.0f, 0.0f),
			1.0f, 0.001f, 0.0009f,
			glm::vec3(0.0f, 3.0f, -5.0f),
			"Dim red lamp"
		)
	);
	
	lights.push_back(
		new PointLight(
			glm::vec3(0.0f),
			glm::vec3(0.0f, 0.0f, 0.2f),
			glm::vec3(0.0f, 0.0f, 0.2f),
			1.0f, 0.001f, 0.0009f,
			glm::vec3(0.0f, 3.0f, -3.0f),
			"Dim green lamp"
		)
	);
	
	lights.push_back(
		new PointLight(
			glm::vec3(0.0f),
			glm::vec3(0.0f, 0.1f, 0.0f),
			glm::vec3(0.0f, 0.1f, 0.0f),
			1.0f, 0.001f, 0.0009f,
			glm::vec3(0.0f, 3.0f, -1.0f),
			"Dim blue lamp"
		)
	);
	
	SpotLight* flashLight = new SpotLight(
		glm::radians(15.f), glm::radians(30.f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.7f, 0.7f, 0.6f),
		glm::vec3(0.8f, 0.8f, 0.7f),
		1.0f, 0.05f, 0.009f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"FlashLight"
	);
	lights.push_back(flashLight);

	// vector<Cube> lightCubes;
	// 
	// for (auto && light : lights)
	// {
	// 	PositionedLight* positionedLight;
	// 	if((positionedLight = dynamic_cast<PositionedLight*>(light)) != nullptr)
	// 	{
	// 		lightCubes.emplace_back(
	// 		Cube(positionedLight->getPosition(), glm::vec3(0.25f), glm::vec3(0.0f), positionedLight->getDiffuse())
	// 		);
	// 	}
	// }

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
	
	// Создаём VAO для 2D quad-а:
	unsigned screenQuadVAO, screenQuadVBO, screenQuadEBO;
	vector<float> screenQuadVertexData = StaticFigures::getQuadVertexesWithUV();
	vector<unsigned> quadIndicesData = StaticFigures::getQuadIndices();

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

	// Создаём VAO для 3D quad-а:
	unsigned worldQuadVAO, worldQuadVBO, worldQuadEBO;
	vector<float> worldQuadVertexData = StaticFigures::getWorldQuadVertexesWithNormalsAndUV();

	glGenVertexArrays(1, &worldQuadVAO);
	glGenBuffers(1, &worldQuadVBO);
	glGenBuffers(1, &worldQuadEBO);

	glBindVertexArray(worldQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, worldQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * worldQuadVertexData.size(), worldQuadVertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, worldQuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * quadIndicesData.size(), quadIndicesData.data(), GL_STATIC_DRAW);

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

	// VAO для куба:
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

	// VAO для куба с инвертированными нормалями:
	unsigned cubeWithInvertedNormalsVAO, cubeWithInvertedNormalsVBO, cubeWithInvertedNormalsEBO;

	vector<float> cubeWithInvertedNormalsVertexData = StaticFigures::getCubeVertexesWithInvertedNormalsAndUV();
	
	glGenVertexArrays(1, &cubeWithInvertedNormalsVAO);
	glGenBuffers(1, &cubeWithInvertedNormalsVBO);
	glGenBuffers(1, &cubeWithInvertedNormalsEBO);

	glBindVertexArray(cubeWithInvertedNormalsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeWithInvertedNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeWithInvertedNormalsVertexData.size(), cubeWithInvertedNormalsVertexData.data(), GL_STATIC_DRAW);

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
	ModelTransformations lightCubeTransformation = {
		glm::vec3(-5.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f,
		glm::vec3(0.5f)
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
		flashLight->setPosition(mainCamera.position
			- mainCamera.up * 0.3f
		);
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

		// Отрисовка "земли".
		glm::mat4 groundModel = groundTransformation.createModelMatrixWithTransformations();
		groundQuad_mixLight->use();
		groundQuad_mixLight->setFloatMat4("projectionAndView", pv);
		groundQuad_mixLight->setFloatMat4("model", groundModel);
		groundQuad_mixLight->setFloat("shininess", 64.0f);
		groundQuad_mixLight->setFloatVec3("viewPos", mainCamera.position);

		int activeLights0 = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights0 += lights[i]->useAndReturnSuccess(groundQuad_mixLight, activeLights0);
		}
		groundQuad_mixLight->setInt("lightsCount", activeLights0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundTextureID);
		groundQuad_mixLight->setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, groundSpecularTextureID);
		groundQuad_mixLight->setInt("texture_specular1", 1);

		glBindVertexArray(worldQuadVAO);
		glDrawElements(GL_TRIANGLES, quadIndicesData.size(), GL_UNSIGNED_INT, 0);

		// Отрисовка модели:
		tankBaseTransformations.position = glm::vec3(glm::cos(currentTime) * 2.5f, 0.0f, glm::cos(currentTime) * 2.5f);
		tankBaseTransformations.rotationAngleDegrees = glm::cos(currentTime) * 30.0f;
		glm::mat4 tankBaseModel = tankBaseTransformations.createModelMatrixWithTransformations();
		
		assimpModelWithLightsAndExplosionShader->use();
		assimpModelWithLightsAndExplosionShader->setFloat("uExplosionMagnitude", 0.0f);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("projectionAndView", pv);
		assimpModelWithLightsAndExplosionShader->setFloatMat4("model", tankBaseModel);
		assimpModelWithLightsAndExplosionShader->setFloat("shininess", 64.0f);
		assimpModelWithLightsAndExplosionShader->setFloatVec3("viewPos", mainCamera.position);
		
		int activeLights = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights += lights[i]->useAndReturnSuccess(assimpModelWithLightsAndExplosionShader, activeLights);
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

		// // Отрисовка "светящегося куба".
		// glm::mat4 lightCubeModel = lightCubeTransformation.createModelMatrixWithTransformations();
		// lightCubeWithExplosionShader->use();
		// lightCubeWithExplosionShader->setFloatMat4("projectionAndView", pv);
		// lightCubeWithExplosionShader->setFloatMat4("model", lightCubeModel);
		// lightCubeWithExplosionShader->setFloatVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));
		// glBindVertexArray(cubeVAO);
		// glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);

		// Отрисовка skybox:
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();
		
		// Избавляемся от перемещения камеры, сохраняя поворот
		glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
		
		skyboxShader->setFloatMat4("projection", projection);
		skyboxShader->setFloatMat4("view", skyboxView);
		
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);  // Последующие элементы влияют на Depth
		
		glDepthFunc(GL_LESS);
		// Конец отрисовки skybox-а
		// Конец отрисовки в framebuffer.

		// Отрисовка в frambuffer для размытия:
		bool horizontal = true, firstIteration = true;
		int amount = 5;
		screenRenderQuadShaderWithBlur->use();
		for(unsigned i = 0; i < amount * 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffers[horizontal]);
			screenRenderQuadShaderWithBlur->setInt("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, firstIteration ? frameBufferColorTexturesID[1] : pingpongTextureIds[!horizontal]);

			glBindVertexArray(screenQuadVAO);
			glDrawElements(GL_TRIANGLES, quadIndicesData.size(), GL_UNSIGNED_INT, 0);
			
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
		glBindVertexArray(screenQuadVAO);
		glDrawElements(GL_TRIANGLES, quadIndicesData.size(), GL_UNSIGNED_INT, 0);
		// Конец отрисовки в стандартный framebuffer.

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
