// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Texture.h"

struct Color {
	float r, g, b, a;
};

Color background = { 0.f, 0.f, 0.f, 1.f };

float mixAmmount = 0.5f;

// Callback функция, вызываемая при изменении размеров окна.
void OnResize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	// std::cout << "Resized to " << width << "x" << height << std::endl;
}

// Работа с устройствами ввода
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		background = { 1.f, 0.f, 0.f, 1.f };
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		background = { 0.f, 1.f, 0.f, 1.f };
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		background = { 0.f, 0.f, 1.f, 1.f };
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixAmmount += 0.01f;
		if (mixAmmount > 1) { mixAmmount = 1.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixAmmount -= 0.01f;
		if (mixAmmount < 0) { mixAmmount = 0.0f; }
	}
}

int main() {
	using namespace std;
	// Настройка glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* win = glfwCreateWindow(500, 500, "OpenGL Window",
		NULL, NULL);

	if (win == NULL) {
		cout << "Error while creating the window.";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

	// Получение функций видеокарты
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
		cout << "Error while loading GLAD.";
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, 500, 500);
	// Конец настройки glfw

	// Исходные данные:
	const int numOfVertexes = 4, sizeOfVertex = 9;

	float polygon[numOfVertexes * sizeOfVertex] = {
		//   X       Y       Z           R       G       B       A		     S       T
			0.5f,	0.0f,   0.0f,		1.0f,	0.0f,	0.0f,	1.0f,		1.0f,	0.0f,
			0.0f,	0.5f,	0.0f,		0.0f,	1.0f,	0.0f,	1.0f,		0.5f,	1.0f,
		   -0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	1.0f,		0.0f,	0.0f,
		    0.0f,  -0.5f,	0.0f,		0.0f,	1.0f,	0.0f,	1.0f,		0.5f,	1.0f,
	};

	const int numOfIndices = 2;

	unsigned indices[numOfIndices * 3] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Конец исходных данных

	unsigned VAO_polygon, VBO_polygon, EBO_polygon;
	// VAO полигона - хранение данных о атрибутах
	// VBO полигона - хранение данных в видеопамяти
	// EBO полигона - ...
	glGenBuffers(1, &VBO_polygon);
	glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	// Привязка VAO:
	glBindVertexArray(VAO_polygon);

	// Сохраняем в VAO:
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);  // Буффер для хранения данных
	// Читаем данные в VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertexes * sizeOfVertex, polygon, GL_STATIC_DRAW);

	// EBO:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numOfIndices * 3, indices, GL_STATIC_DRAW);

	// Сохраняем данные о атрибутах:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Сбрасываем VAO:
	glBindVertexArray(0);

	Shader *basicShader = new Shader("./shaders/basic.vert", "./shaders/basic.frag");
	Texture* containerTexture = new Texture("./textures/container.jpg");
	Texture* pogfaceTexture = new Texture("./textures/awesomeface.png", TextureType::RGBA);

	float dx, dy;
	int drawMode = 2;  // Контролирует отрисовку: 0 - рисование цветом, 1 - рисование текстурой, 2 - две текстуры


	// Устанавливаем ID-текстур для рисования двух текстур сразу
	basicShader->use();
	basicShader->setInt("uTexture", 0);  // GL_TEXTURE0
	basicShader->setInt("uTexture2", 1); // GL_TEXTURE1

	while (!glfwWindowShouldClose(win)) {
		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT);

		dx = sinf(glfwGetTime()) / 2;
		dy = cosf(glfwGetTime()) / 2;

		// drawMode = int(glfwGetTime()) % 2 == 0 ? 1 : 0;

		// containerTexture->use();
		basicShader->use();

		glActiveTexture(GL_TEXTURE0);
		containerTexture->use();
		glActiveTexture(GL_TEXTURE1);
		pogfaceTexture->use();

		basicShader->setInt("uDrawMode", drawMode);
		basicShader->setFloatVec3("uRelativePosition", dx, dy, 0.0f);
		basicShader->setFloat("uMixAmmount", mixAmmount);
		glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, numOfIndices * 3, GL_UNSIGNED_INT, 0);



		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}