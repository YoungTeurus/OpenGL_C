// GLAD ���������� ����� ����� ������������ � ������������ OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "PointLight.h"
#include "classes/Shader.h"
#include "classes/Camera.h"
#include "classes/Texture.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "SpotLight.h"

float deltaTime = 0.0f;									 // ������� �� ������� ����� ��������� � ������������� �������
float lastFrameTime = 0.0f;								 // ����� ���������� �����

int windowInitialWidth = 600, windowInitialHeight = 600; // ��������� ������� ����

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // ���������� ��������� ������� (��������� - �� ������ ����)

bool firstMouse = true;

Camera mainCamera = Camera((float)windowInitialWidth / windowInitialHeight, glm::vec3(0.0f, 0.0f, 5.0f));

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

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// ����� ��������� glfw
#pragma endregion

	Material cubeMaterials[3] = {
		{
			glm::vec3(0.25, 0.20725, 0.20725),
			glm::vec3(1, 0.829, 0.829),
			glm::vec3(0.296648,	0.296648, 0.296648),
			12.f
		}, // pearl
		{
			glm::vec3(0.25, 0.25, 0.25),
			glm::vec3(0.4, 0.4, 0.4),
			glm::vec3(0.774597,	0.774597, 0.774597),
			77.f
		}, // chrome
		{
			glm::vec3(0.1745, 0.01175, 0.01175),
			glm::vec3(0.61424, 0.04136, 0.04136),
			glm::vec3(0.727811, 0.626959, 0.626959),
			77.f
		} // ruby
	};

	const int cube_count = 200;

	ModelTransform cubeTrans[cube_count];
	int cubeMat[cube_count];
	for (int i = 0; i < cube_count; i++)
	{
		float scale = (rand() % 6 + 1) / 20.0f;
		cubeTrans[i] = {
			glm::vec3((rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f),
			glm::vec3(rand() / 100.0f, rand() / 100.0f, rand() / 100.0f),
			glm::vec3(scale, scale, scale)
		};
		cubeMat[i] = rand() % 3;

		if ((glm::vec3(0, 0, 0) - cubeTrans[i].position).length() < 0.7f)
			i--;
	}

	ModelTransform lightTrans = {
		glm::vec3(0.f, 0.f, 0.f),			// position
		glm::vec3(0.f, 0.f, 0.f),			// rotation
		glm::vec3(0.01, 0.01f, 0.01f) };	// scale

	// �������� ������� ������:
	Shader* backpack_shader = new Shader("shaders/backpack_mixLight.vert", "shaders/backpack_mixLight_new.frag");
	
	Model backpack("models/backpack/backpack.obj", false);

	// ���������� ���������� ���������:
	vector<BaseLight*> lights;

	PointLight* redLamp = new PointLight(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 0.2f, 0.2f),
		glm::vec3(1.0f, 0.2f, 0.2f),
		1.0f, 0.1f, 0.09f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"RedLamp"
	);
	lights.push_back(redLamp);
	
	PointLight* blueLamp = new PointLight(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.2f, 0.2f, 1.0f),
		glm::vec3(0.2f, 0.2f, 1.0f),
		1.0f, 0.1f, 0.09f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"BlueLamp"
	);
	lights.push_back(blueLamp);
	
	DirectionalLight* sunLight = new DirectionalLight(
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		"Sun"
	);
	lights.push_back(sunLight);
	
	SpotLight* flashLight = new SpotLight(
		glm::radians(10.f), glm::radians(20.f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.6f),
		glm::vec3(0.8f, 0.8f, 0.6f),
		1.0f, 0.1f, 0.09f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"FlashLight"
	);
	lights.push_back(flashLight);

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

		// // ��������� ��������� ���������� �����:
		// flashLight->setPosition(mainCamera.position - mainCamera.up * 0.3f);
		// flashLight->setDirection(mainCamera.front);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �������� ������:
		view = mainCamera.getViewMatrix();

		// �������� �� FOV ������:
		projection = mainCamera.getProjectionMatrix();

		glm::mat4 pv = projection * view;


		// ��������� �������:
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		backpack_shader->use();
		backpack_shader->setFloatMat4("perspectiveAndView", glm::value_ptr(pv));
		backpack_shader->setFloatMat4("model", glm::value_ptr(model));
		backpack_shader->setFloat("shininess", 64.0f);
		backpack_shader->setFloatVec3("viewPos", mainCamera.position);

		int activeLights = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			activeLights += lights[i]->useAndReturnSuccess(backpack_shader, activeLights);
		}
		backpack_shader->setInt("lightsCount", activeLights);
		backpack.draw(*backpack_shader);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	delete backpack_shader;

	glfwTerminate();
	return 0;
}
