// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct Color {
	float r, g, b, a;
};

Color background = { 0.f, 0.f, 0.f, 1.f };

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
}

int main() {
	using namespace std;
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

	while (!glfwWindowShouldClose(win)) {
		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}