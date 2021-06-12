// GLAD ���������� ����� ����� ������������ � ������������ OpenGL
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

float deltaTime = 0.0f;									 // ������� �� ������� ����� ��������� � ������������� �������
float lastFrameTime = 0.0f;								 // ����� ���������� �����

int windowInitialWidth = 600, windowInitialHeight = 600; // ��������� ������� ����

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // ���������� ��������� ������� (��������� - �� ������ ����)

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

struct ModelTransform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void setScale(float s)
	{
		scale = glm::vec3(s);
	}
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

// Callback �������, ���������� ��� ��������� �������� ����.
void OnResize(GLFWwindow* window, int width, int height)
{
	mainCamera.aspectRatio = (float)width / height;
	glViewport(0, 0, width, height);
}

// ������ � ������������ �����
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}

	// ��������� ����� ���� (��� �������):
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
		background = {1.f, 0.f, 0.f, 1.f};
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		background = {0.f, 1.f, 0.f, 1.f};
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
		background = {0.f, 0.f, 1.f, 1.f};
	}

	// ����������� ������:
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

	// TODO: ������� ������� ������ ������ ��� Z
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::ROLL_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		mainCamera.handleKeyboard(MovementDirection::ROLL_RIGHT, deltaTime);
	}

	// ��������-����� ������:
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

// ��������� �������� ������� ����
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	mainCamera.handleMouseScroll((float)yOffset);
}

// ��������� �������� ����
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	// ��������� ������� �������� ������:
	if (firstMouse) {
		lastCursorX = (float)xpos;
		lastCursorY = (float)ypos;
		firstMouse = false;
	}

	// ������������ �������� ������� ������������ ����������� �����:
	float dx = (float)xpos - lastCursorX,
		  dy = lastCursorY - (float)ypos;  // �������� Y, ��� ��� � OpenGl ��� Y ��� �����, � � ����� - ����
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
	// ��������� glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	// ��������� ������� ����������
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		cout << "Error while loading GLAD.";
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // ����������� ������
	glfwSetCursorPosCallback(win, mouseCallback);  // �������, ���������� ��� ����������� �������
	glfwSetScrollCallback(win, scrollCallback);
	glfwSetKeyCallback(win, onKeyAction);
	setPolygoneDrawMode();

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, windowWidth, windowHeight);

	// TODO: ������� ������������ ������.
	// glEnable(GL_CULL_FACE);
	// ����� ��������� glfw
#pragma endregion

	// const int cube_count = 200;
	// 
	// ModelTransform cubeTrans[cube_count];
	// int cubeMat[cube_count];
	// for (int i = 0; i < cube_count; i++)
	// {
	// 	float scale = (rand() % 6 + 1) / 20.0f;
	// 	cubeTrans[i] = {
	// 		glm::vec3((rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f),
	// 		glm::vec3(rand() / 100.0f, rand() / 100.0f, rand() / 100.0f),
	// 		glm::vec3(scale, scale, scale)
	// 	};
	// 	cubeMat[i] = rand() % 3;
	// 
	// 	if ((glm::vec3(0, 0, 0) - cubeTrans[i].position).length() < 0.7f)
	// 		i--;
	// }

	// ModelTransform lightTrans = {
	// 	glm::vec3(0.f, 0.f, 0.f),			// position
	// 	glm::vec3(0.f, 0.f, 0.f),			// rotation
	// 	glm::vec3(0.01, 0.01f, 0.01f) };	// scale

	// �������� ������� ������:
	Shader* backpackShader = new Shader("backpack_mixLight");
	Shader* lightCubeShader = new Shader("lightCube");
	Shader* singleColorShader = new Shader("shaderSingleColor");
	Shader* screenRenderQuadShader = new Shader("screenRenderQuadShader");
	Shader* skyboxShader = new Shader("skybox");
	
	Model tankModel("models/tank/IS4.obj", true);

	// �������� Cubemap-�:
	std::vector<string> cubeMapFacesTexturePaths = {
		R"(./textures/skybox/right.jpg)",
		R"(./textures/skybox/left.jpg)",
		R"(./textures/skybox/top.jpg)",
		R"(./textures/skybox/bottom.jpg)",
		R"(./textures/skybox/front.jpg)",
		R"(./textures/skybox/back.jpg)",
	};

	unsigned cubeMapTextureId = loadCubeMapFromPathsAndGetTextureId(cubeMapFacesTexturePaths);

	// ���������� ���������� ���������:
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

	// ��������� � ��������:
	unsigned frameBuffer;
	// �������� ������������:
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// �������� ��������, �� �������� ����� ��������� ������ frameBuffer-�.
	unsigned texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// ������������ ��������� �������� � framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// �������� render-������ ��� depth � stencil ������:
	// Render-����� �������� � ������ write-only.
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// ������������ ��������� buffer � framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// ������������� �� ����������� framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ������ VAO ��� quad-�:
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

	
	// ������ VAO ��� skybox-�:
	std::vector<float> skyboxVertexesData = {
			//	  X		 Y		 Z	
			// �������� ����� (z: +1)
				 1.0f,	1.0f,	1.0f,
				-1.0f,	1.0f,	1.0f,
				-1.0f, -1.0f,	1.0f,
				 1.0f, -1.0f,	1.0f,
			// ������ ����� (x: +1)
				 1.0f,	1.0f,  -1.0f,
				 1.0f,	1.0f,	1.0f,
				 1.0f, -1.0f,	1.0f,
				 1.0f, -1.0f,  -1.0f,
			// ������� ����� (y: +1)
				 1.0f,	1.0f,  -1.0f,
				-1.0f,	1.0f,  -1.0f,
				-1.0f,	1.0f,   1.0f,
				 1.0f,	1.0f,   1.0f,
			// ����� ����� (x: -1)
				-1.0f,	1.0f,   1.0f,
				-1.0f,	1.0f,  -1.0f,
				-1.0f, -1.0f,  -1.0f,
				-1.0f, -1.0f,   1.0f,
			// ������ ����� (y: -1)
				 1.0f, -1.0f,   1.0f,
				-1.0f, -1.0f,   1.0f,
				-1.0f, -1.0f,  -1.0f,
				 1.0f, -1.0f,  -1.0f,
			// ������ ����� (z: -1)
				-1.0f,	1.0f,  -1.0f,
				 1.0f,	1.0f,  -1.0f,
				 1.0f, -1.0f,  -1.0f,
				-1.0f, -1.0f,  -1.0f,
		};
	
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
	

	// Model matrix: ���������� ������� � ������� �����������:
	glm::mat4 model;
	// View matrix: ���������� ���� ������������ ������:
	glm::mat4 view;
	// Projection matrix: �������� �������� �� ����������� � �� ��������
	glm::mat4 projection;

	// glm::mat4 transformation;

	while (!glfwWindowShouldClose(win))
	{
		float currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		processInput(win);

		// ��������� ��������� ���������� �����:
		flashLight->setPosition(mainCamera.position - mainCamera.up * 0.3f);
		flashLight->setDirection(mainCamera.front);

		// mainCamera.pitch -= 180.0f;
		mainCamera.yaw += 180.0f;
		mainCamera.handleMouseMovement(0, 0, false);
		
		// �������� ������:
		view = mainCamera.getViewMatrix();

		// �������� �� FOV ������:
		projection = mainCamera.getProjectionMatrix();

		glm::mat4 pv = projection * view;

		// mainCamera.pitch += 180.0f;
		mainCamera.yaw -= 180.0f;
		mainCamera.handleMouseMovement(0, 0, false);

		// ��������� � framebuffer:
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		
		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// ��������� ���������� �����:
		
		// glEnable(GL_DEPTH_TEST); // <- ���������� ����
		// ���� ������ Stencil test, ��������� ������. ��� ��������.
		// ���� ������ Depth test, ��������� ������. � Stencil ��������.
		// ���� ������ ��� �����, ��������� glStencilFunc, ������� ���
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilMask(0x00);  // ��������� ������ � stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // �������, ��� �� ������ ����� ��������� stencil-����

		// ��������� �������:
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		backpackShader->use();
		backpackShader->setFloatMat4("perspectiveAndView", pv);
		backpackShader->setFloatMat4("model", model);
		backpackShader->setFloat("shininess", 64.0f);
		backpackShader->setFloatVec3("viewPos", mainCamera.position);
		
		int activeLights = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights += lights[i]->useAndReturnSuccess(backpackShader, activeLights);
		}
		backpackShader->setInt("lightsCount", activeLights);
		
		tankModel.draw(*backpackShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f,0.8f,0.8f));
		backpackShader->setFloatMat4("model", model);
		tankModel.draw(*backpackShader);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // �������, ��� �� ������ ����� ��������� stencil-����
		glStencilMask(0xFF);  // ��������� ������ � Stencil �����
		// ���� ������ Stencil test, ��������� ������. ��� ��������.
		// ���� ������ Depth test, ��������� ������. � Stencil ��������.
		// ���� ������ ��� �����, ��������� glStencilFunc, ������� ���

		// ������������ ���� � stencil �����

		// lightCubeShader->use();
		// 
		// for (auto && lightCube : lightCubes)
		// {
		// 	model = glm::mat4(1.0f);
		// 	model = glm::translate(model, lightCube.position);
		// 	model = glm::scale(model, lightCube.scale);
		// 	lightCubeShader->setFloatMat4("projectionAndView", pv);
		// 	lightCubeShader->setFloatMat4("model", model);
		// 	lightCubeShader->setFloatVec3("uColor", lightCube.color);
		// 	glBindVertexArray(cubeVAO);
		// 	glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		// 	// lightCube.draw(*lightCubeShader);
		// }

		// ������ stencil ����� �������� 1-�� ���, ��� ����� ����

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // �������, ��� �� ����� ��������� ����, ������ ���� stencil buffer != 1, �.�. ���� ���� �٨ ������ �� �������� (�� �������� ��� ���������)!
		glStencilMask(0x00);  // ��������� ������ � stencil buffer
		glDisable(GL_DEPTH_TEST);  // ���������� ���� ������� (������ ������ ��� ������ �������)

		// ����������� ���� ����� ���������� ������ ���� ��������, �� �� � ��� ������, ��� ����� ��� ����������� ������
		
		// singleColorShader->use();
		// 
		// for (auto && lightCube : lightCubes)
		// {
		// 	model = glm::mat4(1.0f);
		// 	model = glm::translate(model, lightCube.position);
		// 	model = glm::scale(model, lightCube.scale + glm::vec3(0.1));
		// 	singleColorShader->setFloatMat4("projectionAndView", pv);
		// 	singleColorShader->setFloatMat4("model", model);
		// 	glBindVertexArray(cubeVAO);
		// 	glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		// 	// lightCube.draw(*lightCubeShader); 
		// }

		// ��� ���������� �������� ��������� �� ������ � stencil buffer � �������, ��� ��� ������ ����� ��� ���������, � ����� �������� depth_test

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// ��������� skybox:
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();

		glm::mat4 skyboxView = glm::mat4(glm::mat3(mainCamera.getViewMatrix()));

		skyboxShader->setFloatMat4("projection", projection);
		skyboxShader->setFloatMat4("view", skyboxView);

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);  // ����������� �������� ������ �� Depth

		glDepthFunc(GL_LESS);
		// ����� ��������� skybox-�

		// ��������� � ����������� framebuffer:
		view = mainCamera.getViewMatrix();

		pv = projection * view;
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		
		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// ��������� ���������� �����:
		
		// glEnable(GL_DEPTH_TEST); // <- ���������� ����
		// ���� ������ Stencil test, ��������� ������. ��� ��������.
		// ���� ������ Depth test, ��������� ������. � Stencil ��������.
		// ���� ������ ��� �����, ��������� glStencilFunc, ������� ���
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilMask(0x00);  // ��������� ������ � stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // �������, ��� �� ������ ����� ��������� stencil-����

		// ��������� �������:
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		backpackShader->use();
		backpackShader->setFloatMat4("perspectiveAndView", pv);
		backpackShader->setFloatMat4("model", model);
		backpackShader->setFloat("shininess", 64.0f);
		backpackShader->setFloatVec3("viewPos", mainCamera.position);
		
		activeLights = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights += lights[i]->useAndReturnSuccess(backpackShader, activeLights);
		}
		backpackShader->setInt("lightsCount", activeLights);
		
		tankModel.draw(*backpackShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f,0.8f,0.8f));
		backpackShader->setFloatMat4("model", model);
		tankModel.draw(*backpackShader);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // �������, ��� �� ������ ����� ��������� stencil-����
		glStencilMask(0xFF);  // ��������� ������ � Stencil �����
		// ���� ������ Stencil test, ��������� ������. ��� ��������.
		// ���� ������ Depth test, ��������� ������. � Stencil ��������.
		// ���� ������ ��� �����, ��������� glStencilFunc, ������� ���

		// ������������ ���� � stencil �����

		// lightCubeShader->use();
		// 
		// for (auto && lightCube : lightCubes)
		// {
		// 	model = glm::mat4(1.0f);
		// 	model = glm::translate(model, lightCube.position);
		// 	model = glm::scale(model, lightCube.scale);
		// 	lightCubeShader->setFloatMat4("projectionAndView", pv);
		// 	lightCubeShader->setFloatMat4("model", model);
		// 	lightCubeShader->setFloatVec3("uColor", lightCube.color);
		// 	glBindVertexArray(cubeVAO);
		// 	glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		// 	// lightCube.draw(*lightCubeShader);
		// }

		// ������ stencil ����� �������� 1-�� ���, ��� ����� ����

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // �������, ��� �� ����� ��������� ����, ������ ���� stencil buffer != 1, �.�. ���� ���� �٨ ������ �� �������� (�� �������� ��� ���������)!
		glStencilMask(0x00);  // ��������� ������ � stencil buffer
		glDisable(GL_DEPTH_TEST);  // ���������� ���� ������� (������ ������ ��� ������ �������)

		// ����������� ���� ����� ���������� ������ ���� ��������, �� �� � ��� ������, ��� ����� ��� ����������� ������
		
		// singleColorShader->use();
		// 
		// for (auto && lightCube : lightCubes)
		// {
		// 	model = glm::mat4(1.0f);
		// 	model = glm::translate(model, lightCube.position);
		// 	model = glm::scale(model, lightCube.scale + glm::vec3(0.1));
		// 	singleColorShader->setFloatMat4("projectionAndView", pv);
		// 	singleColorShader->setFloatMat4("model", model);
		// 	glBindVertexArray(cubeVAO);
		// 	glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		// 	// lightCube.draw(*lightCubeShader); 
		// }

		// ��� ���������� �������� ��������� �� ������ � stencil buffer � �������, ��� ��� ������ ����� ��� ���������, � ����� �������� depth_test

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// ��������� skybox:
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();

		skyboxView = glm::mat4(glm::mat3(mainCamera.getViewMatrix()));

		skyboxShader->setFloatMat4("projection", projection);
		skyboxShader->setFloatMat4("view", skyboxView);

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, cubeIndicesData.size(), GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);  // ����������� �������� ������ �� Depth

		glDepthFunc(GL_LESS);
		// ����� ��������� skybox-�
		

		screenRenderQuadShader->use();
		glBindVertexArray(screenQuadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawElements(GL_TRIANGLES, screenQuadIndicesData.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// delete backpackShader;

	glfwTerminate();
	return 0;
}
