// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
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

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

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

int main()
{
	using namespace std;
#pragma region GLFW INIT
	// Настройка glfw
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Конец настройки glfw
#pragma endregion

	// Загрузка внешних данных:
	Shader* backpack_shader = new Shader("shaders\\backpack_mixLight.vert", "shaders\\backpack_mixLight.frag");
	
	Model backpack("models/backpack/backpack.obj", false);

	// Подготовка источников освещения:
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
		glm::radians(10.f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.6f),
		glm::vec3(0.8f, 0.8f, 0.6f),
		1.0f, 0.1f, 0.09f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		"FlashLight"
	);
	lights.push_back(flashLight);

	// Model matrix: размещение объекта в мировых координатах:
	glm::mat4 model;
	// View matrix: размещение мира относительно камеры:
	glm::mat4 view;
	// Projection matrix: поправка объектов на перспективу и их клиппинг
	glm::mat4 projection;

	// glm::mat4 transformation;

	while (!glfwWindowShouldClose(win))
	{
		float currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Изменение положения источников света:
		flashLight->setPosition(mainCamera.position - mainCamera.up * 0.3f);
		flashLight->setDirection(mainCamera.front);

		// Вращение камеры:
		view = mainCamera.getViewMatrix();

		// Поправка на FOV камеры:
		projection = mainCamera.getProjectionMatrix();

		glm::mat4 pv = projection * view;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
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
