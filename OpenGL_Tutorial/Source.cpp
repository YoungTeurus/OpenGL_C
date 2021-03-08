// GLAD ���������� ����� ����� ������������ � ������������ OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

struct Color {
	float r, g, b, a;
};

Color background = { 0.f, 0.f, 0.f, 1.f };

// Callback �������, ���������� ��� ��������� �������� ����.
void OnResize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	// std::cout << "Resized to " << width << "x" << height << std::endl;
}

// ������ � ������������ �����
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
	// ��������� glfw
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

	// ��������� ������� ����������
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
		cout << "Error while loading GLAD.";
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(win, OnResize);

	glViewport(0, 0, 500, 500);
	// ����� ��������� glfw

	const int numOfVertexes = 4, sizeOfVertex = 7;

	float polygon[numOfVertexes * sizeOfVertex] = {
			0.5f,	0.0f,   0.0f,			1.0f,	0.0f,	0.0f,	1.0f,
			0.0f,	0.5f,	0.0f,			0.0f,	1.0f,	0.0f,	1.0f,
		   -0.5f,	0.0f,	0.0f,			0.0f,	0.0f,	1.0f,	1.0f,
		    0.0f,  -0.5f,	0.0f,			0.0f,	1.0f,	0.0f,	1.0f,
	};

	const int numOfIndices = 2;

	unsigned indices[numOfIndices * 3] = {
		0, 1, 2,
		0, 2, 3,
	};

	unsigned VAO_polygon, VBO_polygon, EBO_polygon;
	// VAO �������� - �������� ������ � ���������
	// VBO �������� - �������� ������ � �����������
	// EBO �������� - ...
	glGenBuffers(1, &VBO_polygon);
	glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	// �������� VAO:
	glBindVertexArray(VAO_polygon);

	// ��������� � VAO:
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);  // ������ ��� �������� ������
	// ������ ������ � VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertexes * sizeOfVertex, polygon, GL_STATIC_DRAW);

	// EBO:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numOfIndices * 3, indices, GL_STATIC_DRAW);

	// ��������� ������ � ���������:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeOfVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ���������� VAO:
	glBindVertexArray(0);

	Shader *basic = new Shader("./shaders/basic.vert", "./shaders/basic.frag");

	while (!glfwWindowShouldClose(win)) {
		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT);



		basic->use();
		glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, numOfIndices * 3, GL_UNSIGNED_INT, 0);



		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}