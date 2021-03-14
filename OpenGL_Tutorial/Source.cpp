// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "classes/Shader.h"
#include "classes/Camera.h"
#include "classes/Texture.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = (float)windowInitialWidth / 2,
	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

bool firstMouse = true;

Camera mainCamera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

struct Color
{
	float r, g, b, a;
};

Color background = {0.05f, 0.05f, 0.05f, 1.f};

// Callback функция, вызываемая при изменении размеров окна.
void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// std::cout << "Resized to " << width << "x" << height << std::endl;
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

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, windowWidth, windowHeight);
	// Конец настройки glfw
#pragma endregion

	// Исходные данные:
	const int numOfVertexes_cube = 24, sizeOfVertex_cube = 12;

	float cubeMesh[numOfVertexes_cube * sizeOfVertex_cube] = {
		//   X       Y       Z           R       G       B       A			 Nx	      Ny       Nz          S       T
		// Передняя грань (z: +1)
		    1.0f,   1.0f,   1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    0.0f,    1.0f,       1.0f,   1.0f,  // верхний правый
		   -1.0f,   1.0f,   1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    0.0f,    1.0f,       0.0f,   1.0f,  // верхний левый
		   -1.0f,  -1.0f,   1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	    0.0f,    0.0f,    1.0f,       0.0f,   0.0f,  // нижний левый
		    1.0f,  -1.0f,   1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	    0.0f,    0.0f,    1.0f,       1.0f,   0.0f,  // нижний правый
		// Правая грань (x: +1)											    
			1.0f,   1.0f,  -1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	    1.0f,    0.0f,    0.0f,       1.0f,   1.0f,  // верхний правый
			1.0f,   1.0f,   1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	    1.0f,    0.0f,    0.0f,       0.0f,   1.0f,  // верхний левый
			1.0f,  -1.0f,   1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	    1.0f,    0.0f,    0.0f,       0.0f,   0.0f,  // нижний левый
			1.0f,  -1.0f,  -1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	    1.0f,    0.0f,    0.0f,       1.0f,   0.0f,  // нижний правый
		// Верхняя грань (y: +1)										   
			1.0f,   1.0f,  -1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    1.0f,    0.0f,       1.0f,   1.0f,  // верхний правый
		   -1.0f,   1.0f,  -1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    1.0f,    0.0f,       0.0f,   1.0f,  // верхний левый
		   -1.0f,   1.0f,   1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	    0.0f,    1.0f,    0.0f,       0.0f,   0.0f,  // нижний левый
			1.0f,   1.0f,   1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	    0.0f,    1.0f,    0.0f,       1.0f,   0.0f,  // нижний правый
		// Левая грань (x: -1)											    
		   -1.0f,   1.0f,   1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	   -1.0f,    0.0f,    0.0f,       1.0f,   1.0f,  // верхний правый
		   -1.0f,   1.0f,  -1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	   -1.0f,    0.0f,    0.0f,       0.0f,   1.0f,  // верхний левый
		   -1.0f,  -1.0f,  -1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	   -1.0f,    0.0f,    0.0f,       0.0f,   0.0f,  // нижний левый
		   -1.0f,  -1.0f,   1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	   -1.0f,    0.0f,    0.0f,       1.0f,   0.0f,  // нижний правый
		// Нижняя грань (y: -1)											    
			1.0f,  -1.0f,   1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	    0.0f,   -1.0f,    0.0f,       1.0f,   1.0f,  // верхний правый
		   -1.0f,  -1.0f,   1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	    0.0f,   -1.0f,    0.0f,       0.0f,   1.0f,  // верхний левый
		   -1.0f,  -1.0f,  -1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	    0.0f,   -1.0f,    0.0f,       0.0f,   0.0f,  // нижний левый
			1.0f,  -1.0f,  -1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	    0.0f,   -1.0f,    0.0f,       1.0f,   0.0f,  // нижний правый
		// Задняя грань (z: -1)											   
		   -1.0f,   1.0f,  -1.0f,       1.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    0.0f,   -1.0f,       1.0f,   1.0f,  // верхний правый
			1.0f,   1.0f,  -1.0f,       0.0f,   1.0f,   1.0f,   1.0f,	    0.0f,    0.0f,   -1.0f,       0.0f,   1.0f,  // верхний левый
			1.0f,  -1.0f,  -1.0f,       1.0f,   0.0f,   1.0f,   1.0f,	    0.0f,    0.0f,   -1.0f,       0.0f,   0.0f,  // нижний левый
		   -1.0f,  -1.0f,  -1.0f,       0.5f,   0.5f,   0.5f,   1.0f,	    0.0f,    0.0f,   -1.0f,       1.0f,   0.0f,  // нижний правый
	};

	const int cubeIndicesCount = 12;

	unsigned cubeIndices[cubeIndicesCount * 3] = {
		// Одна грань - 2 треугольника - 6 точек
		 0, 1, 2,	 0, 2, 3,
		 4, 5, 6,	 4, 6, 7,
		 8, 9,10,	 8,10,11,
		12,13,14,	12,14,15,
	    16,17,18,	16,18,19,
		20,21,22,	20,22,23,
	};

	// float polygon[numOfVertexes * sizeOfVertex] = {
	// 	//   X       Y       Z           R       G       B       A		     S       T
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,						0.0f, 0.0f,
	// 	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,							1.0f, 0.0f,
	// 	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,							1.0f, 1.0f,
	// 	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,							1.0f, 1.0f,
	// 	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,							0.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,						0.0f, 0.0f,
	// 
	// 	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 
	// 	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 
	// 	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// 	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// };

	glm::vec3 cubePositions[] = {
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	float cubeScales[] = {
		0.4f,
		0.6f,
		0.55f,
		0.35f,
		0.75f,
		0.25f,
		0.85f,
		0.6f,
		0.3f,
	};
	glm::vec3 cubeColor[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.3f, 0.0f),
		glm::vec3(0.4f, 0.3f, 0.5f),
		glm::vec3(0.3f, 0.6f, 0.1f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.8f, 0.1f, 0.8f),
	};

	glm::vec3 lightCubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
	};
	float lightCubeScales[] = {
		0.3f,
	};

	// Конец исходных данных

#pragma region Cube VAO, VBO, EBO
	unsigned	 VAO_cube
				,VBO_cube
				,EBO_cube
				;
	// VAO полигона - хранение данных о атрибутах
	// VBO полигона - хранение данных в видеопамяти
	// EBO полигона - хранение данных о рёбрах и гранях
	glGenBuffers(1, &VBO_cube);
	glGenBuffers(1, &EBO_cube);
	glGenVertexArrays(1, &VAO_cube);

	// Привязка VAO:
	glBindVertexArray(VAO_cube);

	// Сохраняем в VAO:
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube); // Буффер для хранения данных
	// Читаем данные в VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertexes_cube * sizeOfVertex_cube, cubeMesh, GL_STATIC_DRAW);

	// EBO:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * cubeIndicesCount * 3, cubeIndices, GL_STATIC_DRAW);

	// Сохраняем данные о атрибутах:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Сбрасываем VAO:
	glBindVertexArray(0);
#pragma endregion

#pragma region Light VAO, VBO, EBO
	unsigned VAO_light;
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	// Повторное использование EBO куба:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);

	// Повторное использование VBO куба:
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);

	// Записываем в новый VAO данные из старого VBO:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
#pragma endregion

	glEnable(GL_DEPTH_TEST);

	// Shader* basicShader = new Shader("./shaders/basic.vert", "./shaders/basic.frag");
	Shader* cubeShader = new Shader("./shaders/cube.vert", "./shaders/cube_spotLight.frag");
	Shader* lightShader = new Shader("./shaders/cube.vert", "./shaders/lightCube.frag");

	Texture* containerTexture = new Texture("./textures/container.jpg");
	Texture* container2Texture = new Texture("./textures/container2.png", TextureType::RGBA);
	Texture* container2_specularTexture = new Texture("./textures/container2_specular.png", TextureType::RGBA);
	Texture* pogfaceTexture = new Texture("./textures/awesomeface.png", TextureType::RGBA);
	Texture* matrixTexture = new Texture("./textures/matrix.jpg");

	int drawMode; // Контролирует отрисовку: 0 - рисование цветом, 1 - рисование текстурой, 2 - две текстуры


	// // Устанавливаем ID-текстур для рисования двух текстур сразу
	// basicShader->use();
	// basicShader->setInt("uTexture", 0); // GL_TEXTURE0
	// basicShader->setInt("uTexture2", 1); // GL_TEXTURE1


	// Model matrix: размещение объекта в мировых координатах:
	glm::mat4 model;
	// View matrix: размещение мира относительно камеры:
	glm::mat4 view;
	// Projection matrix: поправка объектов на перспективу и их клиппинг
	glm::mat4 projection;

	glm::mat4 transformation;

	while (!glfwWindowShouldClose(win))
	{
		float currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Вращение камеры:
		view = mainCamera.getViewMatrix();

		// Поправка на FOV камеры:
		projection = glm::perspective(glm::radians(mainCamera.FOV), (float)windowWidth / windowHeight, 0.1f, 100.0f);


		// Отрисовка ящиков:
		int i = 0;
		for (auto cubPos : cubePositions)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubPos);
			// model = glm::rotate(model, glm::radians((float)(90.0f * sin(glfwGetTime() * i))), glm::vec3(0.1f, 0.2f, 0.3f));
			model = glm::scale(model, glm::vec3(cubeScales[i], cubeScales[i], cubeScales[i]));

			// Загружаем (биндим) карту диффузии в текстуру 0
			glActiveTexture(GL_TEXTURE0);
			container2Texture->use();
			glActiveTexture(GL_TEXTURE1);
			container2_specularTexture->use();
			// glActiveTexture(GL_TEXTURE2);
			// matrixTexture->use();
			cubeShader->use();
			cubeShader->setFloatVec3(	"material.specular",	0.5f, 0.5f, 0.5f);
			cubeShader->setFloat(		"material.shininess",				32.0f);
			cubeShader->setFloatVec3(	"light.ambient",		0.2f, 0.2f, 0.2f);
			cubeShader->setFloatVec3(	"light.diffuse",		0.5f, 0.5f, 0.5f);
			cubeShader->setFloatVec3(	"light.specular",		1.0f, 1.0f, 1.0f);
			cubeShader->setFloatVec3(	"light.position", mainCamera.position);
			cubeShader->setFloatVec3(	"light.direction", mainCamera.front);
			cubeShader->setFloat(		"light.cutOffCosin", glm::cos(glm::radians(12.5f)));
			cubeShader->setFloat(		"light.constant",					1.0f);
			cubeShader->setFloat(		"light.linear",						0.09f);
			cubeShader->setFloat(		"light.quadratic",					0.032f);
			cubeShader->setInt(			"material.diffuse", 0);  // Указываем, что для карты диффузии используется текстура 0
			cubeShader->setInt(			"material.specular", 1);  // Указываем, что для карты диффузии используется текстура 1
			cubeShader->setInt(			"material.emission", 2);  // Указываем, что для карты свечения используется текстура 2
			
			transformation = projection * view * model;

			// Матрица нормалей:
			glm::mat3 normal = glm::transpose(glm::inverse(model));

			cubeShader->setFloatMat4("model", glm::value_ptr(model));
			cubeShader->setFloatMat4("view", glm::value_ptr(view));
			cubeShader->setFloatMat4("projection", glm::value_ptr(projection));
			cubeShader->setFloatMat4("transformation", glm::value_ptr(transformation));
			cubeShader->setFloatMat3("normal", glm::value_ptr(normal));
			cubeShader->setFloatVec3("viewPos", mainCamera.position);

			glBindVertexArray(VAO_cube);
			glDrawElements(GL_TRIANGLES, cubeIndicesCount * 3, GL_UNSIGNED_INT, 0);

			i++;
		}

		// i = 0;
		// for (auto lightCubPos : lightCubePositions) {
		// 	model = glm::mat4(1.0f);
		// 	model = glm::translate(model, lightCubPos);
		// 	// model = glm::rotate(model, glm::radians((float)(90.0f * sin(glfwGetTime()))), glm::vec3(0.1f, 0.2f, 0.3f));
		// 	model = glm::scale(model, glm::vec3(lightCubeScales[i], lightCubeScales[i], lightCubeScales[i]));
		// 
		// 	lightShader->use();
		// 	
		// 	transformation = projection * view * model;
		// 	
		// 	lightShader->setFloatMat4("transformation", glm::value_ptr(transformation));
		// 
		// 	glBindVertexArray(VAO_light);
		// 	glDrawElements(GL_TRIANGLES, cubeIndicesCount * 3, GL_UNSIGNED_INT, 0);
		// 
		// 	i++;
		// }


		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
