// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Texture.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра

int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна

float lastCursorX = windowInitialWidth / 2,
	  lastCursorY = windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)

glm::vec3 cameraPos		= glm::vec3(0.0f, 0.0f,	 3.0f);  // Положение камеры
glm::vec3 cameraFront	= glm::vec3(0.0f, 0.0f, -1.0f);  // Направление взгляда камеры
glm::vec3 cameraUp		= glm::vec3(0.0f, 1.0f,  0.0f);  // Направление "вверх" для камеры

float cameraYaw = -90.f,  // Рыскание камеры: вращение вокруг оси Y (по часовой стрелке) - влево-вправо
	  cameraPitch = 0.0f; // Тангаж камеры: вращение вокруг оси X (по часовой стрелке) - вниз-вверх
float cameraFOV = 30.0f;  // Угол зрения камеры

const float cameraSpeed = 1.0f;							 // Скорость движения камеры
const float mouseSensitivity = 0.05f;  // Чувствительность мыши

bool firstMouse = true;

struct Color
{
	float r, g, b, a;
};

Color background = {0.f, 0.f, 0.f, 1.f};

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
	float cameraSpeed = ::cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

// Обработка движения колёсика мыши
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	cameraFOV -= (float)yOffset;
	if (cameraFOV < 1.0f)
		cameraFOV = 1.0f;
	else if (cameraFOV > 89.5f)
		cameraFOV = 89.5f;
}

// Обработка движения мыши
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	// Обработка первого движения камеры:
	if (firstMouse) {
		lastCursorX = xpos;
		lastCursorY = ypos;
		firstMouse = false;
	}

	// Подсчитываем смещение курсора относительно предыдущего кадра:
	float dx = xpos - lastCursorX,
		  dy = lastCursorY - ypos;  // Отражаем Y, так как в OpenGl ось Y идёт вверх, а в окнах - вниз
	lastCursorX = xpos; lastCursorY = ypos;

	dx = dx * mouseSensitivity;
	dy = dy * mouseSensitivity;

	// Добавляем смещение к рысканию и тангажу:
	cameraYaw += dx;
	cameraPitch += dy;

	// Корректируем тангаж:
	if (cameraPitch > 89.5f)
		cameraPitch = 89.5f;
	else if (cameraPitch < -89.5f)
		cameraPitch = -89.5;

	// Вычисляем вектор направления камеры:
	glm::vec3 direction;
	direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.y = sin(glm::radians(cameraPitch));
	direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

	cameraFront = glm::normalize(direction);
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
	const int numOfVertexes = 36, sizeOfVertex = 9;

	float polygon[numOfVertexes * sizeOfVertex] = {
		//   X       Y       Z           R       G       B       A		     S       T
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
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

	// const int numOfIndices = 12;
	// 
	// unsigned indices[numOfIndices * 3] = {
	// 	0, 1, 2,
	// 	1, 2, 3,
	// 	0, 2, 4,
	// 	2, 4, 6,
	// 	0, 4, 1,
	// 	4, 1, 5,
	// 	1, 3, 5,
	// 	3, 5, 7,
	// 	2, 3, 6,
	// 	3, 6, 7,
	// 	4, 5, 6,
	// 	5, 6, 7
	// };

	// Конец исходных данных

	unsigned	VAO_polygon
				,VBO_polygon
				// ,EBO_polygon
				;
	// VAO полигона - хранение данных о атрибутах
	// VBO полигона - хранение данных в видеопамяти
	// EBO полигона - ...
	glGenBuffers(1, &VBO_polygon);
	// glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	// Привязка VAO:
	glBindVertexArray(VAO_polygon);

	// Сохраняем в VAO:
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon); // Буффер для хранения данных
	// Читаем данные в VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertexes * sizeOfVertex, polygon, GL_STATIC_DRAW);

	// // EBO:
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numOfIndices * 3, indices, GL_STATIC_DRAW);

	// Сохраняем данные о атрибутах:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Сбрасываем VAO:
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	Shader* basicShader = new Shader("./shaders/basic.vert", "./shaders/basic.frag");
	Texture* containerTexture = new Texture("./textures/container.jpg");
	Texture* pogfaceTexture = new Texture("./textures/awesomeface.png", TextureType::RGBA);

	float dx, dy;
	int drawMode = 1; // Контролирует отрисовку: 0 - рисование цветом, 1 - рисование текстурой, 2 - две текстуры


	// Устанавливаем ID-текстур для рисования двух текстур сразу
	basicShader->use();
	basicShader->setInt("uTexture", 0); // GL_TEXTURE0
	basicShader->setInt("uTexture2", 1); // GL_TEXTURE1


	// Model matrix: размещение объекта в мировых координатах:
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// View matrix: размещение мира относительно камеры:

	// Примитивно:
	// // Отодвинули мир вперёд (Z+ ось), т.е. отодвинули камеру назад.
	// glm::mat4 view = glm::mat4(1.0f);
	// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	// Через матрицу LookAt:
	// // Создаём вектор позиции камеры (отодвинут "назад", "из экрана" - Z+):
	// glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	// // Точка, куда смотрит камера:
	// glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// // Вектор "вверх" для мира:
	// glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	// // (Обратное) направление взгляда камеры:
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget); // Направлена в +Z
	// // Вектор "вправо" для камеры: результат векторного произведения "вверх" и "вперёд" (правило правой руки)
	// glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	// // Вектор "вверх" для камеры:
	// glm::vec3 cameraUp = glm::cross(cameraRight, cameraDirection);
	// // Создаём матрицу LookAt...

	const float radius = 10.0f;

	// glm::mat4 view = glm::lookAt(
	// 	cameraPosition,		// Позиция камеры
	// 	cameraTarget,		// Направление, куда смотрит камера
	// 	worldUp				// Направление "вверх" для мира
	// );

	// Projection matrix: поправка объектов на перспективу и их клиппинг
	// glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(win))
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Вращение камеры:
		glm::mat4 view = glm::lookAt(
			cameraPos,						// Позиция камеры
			cameraPos + cameraFront,		// Положение камеры
			cameraUp						// Направление вверх для камеры
		);

		// Поправка на FOV камеры:
		glm::mat4 projection = glm::perspective(glm::radians(cameraFOV), (float)windowWidth / windowHeight, 0.1f, 100.0f);

		for (auto cubPos : cubePositions)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubPos);
			model = glm::rotate(model, glm::radians((float)(90.0f * sin(glfwGetTime()))), glm::vec3(0.1f, 0.2f, 0.3f));

			containerTexture->use();
			basicShader->use();
			basicShader->setInt("uDrawMode", drawMode);
			basicShader->setFloatMat4("model", glm::value_ptr(model));
			basicShader->setFloatMat4("view", glm::value_ptr(view));
			basicShader->setFloatMat4("projection", glm::value_ptr(projection));

			glBindVertexArray(VAO_polygon);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// glDrawElements(GL_TRIANGLES, numOfIndices * 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
