// // GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
// #include <glad/glad.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <GLFW/glfw3.h>
// #include <iostream>
// 
// #include "Cube.h"
// #include "PointLight.h"
// #include "classes/Shader.h"
// #include "classes/Camera.h"
// #include "classes/Texture.h"
// 
// float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
// float lastFrameTime = 0.0f;								 // Время последнего кадра
// 
// int windowInitialWidth = 600, windowInitialHeight = 600; // Стартовые размеры окна
// 
// float lastCursorX = (float)windowInitialWidth / 2,
// 	  lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)
// 
// bool firstMouse = true;
// 
// Camera mainCamera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
// 
// struct Color
// {
// 	float r, g, b, a;
// };
// 
// Color background = {0.05f, 0.05f, 0.05f, 1.f};
// 
// // Callback функция, вызываемая при изменении размеров окна.
// void OnResize(GLFWwindow* window, int width, int height)
// {
// 	glViewport(0, 0, width, height);
// 	// std::cout << "Resized to " << width << "x" << height << std::endl;
// }
// 
// // Работа с устройствами ввода
// void processInput(GLFWwindow* window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
// 		glfwSetWindowShouldClose(window, true);
// 	}
// 
// 	// Изменение цвета фона (для отладки):
// 	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
// 		background = {1.f, 0.f, 0.f, 1.f};
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
// 		background = {0.f, 1.f, 0.f, 1.f};
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
// 		background = {0.f, 0.f, 1.f, 1.f};
// 	}
// 
// 	// Перемещение камеры:
// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::FORWARD, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::BACKWARD, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::STRAFE_LEFT, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::STRAFE_RIGHT, deltaTime);
// 	}
// 
// 	// TODO: сделать поворот камеры вокруг оси Z
// 	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::ROLL_LEFT, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::ROLL_RIGHT, deltaTime);
// 	}
// 
// 	// Поднятие-спуск камеры:
// 	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::UP, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
// 		mainCamera.handleKeyboard(MovementDirection::DOWN, deltaTime);
// 	}
// }
// 
// // Обработка движения колёсика мыши
// void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
// 	mainCamera.handleMouseScroll((float)yOffset);
// }
// 
// // Обработка движения мыши
// void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
// 	// Обработка первого движения камеры:
// 	if (firstMouse) {
// 		lastCursorX = (float)xpos;
// 		lastCursorY = (float)ypos;
// 		firstMouse = false;
// 	}
// 
// 	// Подсчитываем смещение курсора относительно предыдущего кадра:
// 	float dx = (float)xpos - lastCursorX,
// 		  dy = lastCursorY - (float)ypos;  // Отражаем Y, так как в OpenGl ось Y идёт вверх, а в окнах - вниз
// 	lastCursorX = (float)xpos; lastCursorY = (float)ypos;
// 
// 	mainCamera.handleMouseMovement(dx, dy);
// }
// 
// int main()
// {
// 	using namespace std;
// #pragma region GLFW INIT
// 	// Настройка glfw
// 	glfwInit();
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// 
// 	int windowWidth = windowInitialWidth, windowHeight = windowInitialHeight;
// 
// 	GLFWwindow* win = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Window",
// 	                                   NULL, NULL);
// 
// 	if (win == NULL)
// 	{
// 		cout << "Error while creating the window.";
// 		glfwTerminate();
// 		return -1;
// 	}
// 	glfwMakeContextCurrent(win);
// 
// 	// Получение функций видеокарты
// 	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
// 	{
// 		cout << "Error while loading GLAD.";
// 		glfwTerminate();
// 		return -1;
// 	}
// 
// 	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Захватываем курсор
// 	glfwSetCursorPosCallback(win, mouseCallback);  // Функция, вызываемая при перемещении курсора
// 	glfwSetScrollCallback(win, scrollCallback);
// 
// 	glfwSetFramebufferSizeCallback(win, OnResize);
// 
// 	glViewport(0, 0, windowWidth, windowHeight);
// 	// Конец настройки glfw
// #pragma endregion
// 
// 	glm::vec3 cubePositions[] = {
// 		glm::vec3(2.0f, 5.0f, -15.0f),
// 		glm::vec3(-1.5f, -2.2f, -2.5f),
// 		glm::vec3(-3.8f, -2.0f, -12.3f),
// 		glm::vec3(2.4f, -0.4f, -3.5f),
// 		glm::vec3(-1.7f, 3.0f, -7.5f),
// 		glm::vec3(1.3f, -2.0f, -2.5f),
// 		glm::vec3(1.5f, 2.0f, -2.5f),
// 		glm::vec3(1.5f, 0.2f, -1.5f),
// 		glm::vec3(-1.3f, 1.0f, -1.5f)
// 	};
// 	float cubeScales[] = {
// 		0.4f,
// 		0.6f,
// 		0.55f,
// 		0.35f,
// 		0.75f,
// 		0.25f,
// 		0.85f,
// 		0.6f,
// 		0.3f,
// 	};
// 	glm::vec3 cubeColor[] = {
// 		glm::vec3(1.0f, 0.0f, 0.0f),
// 		glm::vec3(0.7f, 0.3f, 0.0f),
// 		glm::vec3(0.4f, 0.3f, 0.5f),
// 		glm::vec3(0.3f, 0.6f, 0.1f),
// 		glm::vec3(0.8f, 0.8f, 0.8f),
// 		glm::vec3(0.0f, 1.0f, 0.0f),
// 		glm::vec3(0.0f, 0.0f, 1.0f),
// 		glm::vec3(0.5f, 0.5f, 0.5f),
// 		glm::vec3(0.8f, 0.1f, 0.8f),
// 	};
// 
// 	glm::vec3 lightCubePositions[] = {
// 		glm::vec3(0.0f, 0.0f, 10.0f),
// 		glm::vec3(0.0f, 0.0f, -5.0f),
// 		glm::vec3(-2.0f, -2.0f, -5.0f),
// 		glm::vec3(2.0f, 2.0f, -5.0f),
// 	};
// 	glm::vec3 lightCubeAmbient[] = {
// 		glm::vec3(0.05f, 0.05f, 0.05f),
// 		glm::vec3(0.1f, 0.00f, 0.00f),
// 		glm::vec3(0.00f, 0.1f, 0.00f),
// 		glm::vec3(0.00f, 0.00f, 0.1f),
// 	};
// 	glm::vec3 lightCubeDiffuse[] = {
// 		glm::vec3(0.5f, 0.5f, 0.5f),
// 		glm::vec3(1.0f, 0.0f, 0.0f),
// 		glm::vec3(0.0f, 1.0f, 0.0f),
// 		glm::vec3(0.0f, 0.0f, 1.0f),
// 	};
// 	glm::vec3 lightCubeSpecular[] = {
// 		glm::vec3(1.0f, 1.0f, 1.0f),
// 		glm::vec3(1.0f, 0.0f, 0.0f),
// 		glm::vec3(0.0f, 1.0f, 0.0f),
// 		glm::vec3(0.0f, 0.0f, 1.0f),
// 	};
// 	float lightCubeScales[] = {
// 		0.3f,
// 		0.3f,
// 		0.3f,
// 		0.3f,
// 	};
// 
// 	// Конец исходных данных
// 
// 	// Загрузка внешних данных:
// 	// Shader* basicShader = new Shader("./shaders/basic.vert", "./shaders/basic.frag");
// 	Shader* cubeShader = new Shader("./shaders/cube.vert", "./shaders/cube_mixLight.frag");
// 	Shader* lightShader = new Shader("./shaders/cube.vert", "./shaders/lightCube.frag");
// 
// 	Texture* containerTexture = new Texture("./textures/container.jpg");
// 	Texture* container2Texture = new Texture("./textures/container2.png", TextureType::RGBA);
// 	Texture* container2_specularTexture = new Texture("./textures/container2_specular.png", TextureType::RGBA);
// 	Texture* pogfaceTexture = new Texture("./textures/awesomeface.png", TextureType::RGBA);
// 	Texture* matrixTexture = new Texture("./textures/matrix.jpg");
// 	Texture* lampTexture = new Texture("./textures/glowing_lamp.jpg");
// 
// 	std::list<Cube> cubes = {
// 		Cube(cubeShader, new Material()),
// 	};
// 
// 	std::list<Light> lights = {
// 		PointLight(lightShader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
// 		1.0f, 0.09f, 0.032f, 5.0f, 5.0f, 0.0f),
// 	};
// 
// 	glEnable(GL_DEPTH_TEST);
// 
// 	// Model matrix: размещение объекта в мировых координатах:
// 	glm::mat4 model;
// 	// View matrix: размещение мира относительно камеры:
// 	glm::mat4 view;
// 	// Projection matrix: поправка объектов на перспективу и их клиппинг
// 	glm::mat4 projection;
// 
// 	glm::mat4 transformation;
// 
// 	while (!glfwWindowShouldClose(win))
// 	{
// 		float currentFrameTime = (float)glfwGetTime();
// 		deltaTime = currentFrameTime - lastFrameTime;
// 		lastFrameTime = currentFrameTime;
// 
// 		processInput(win);
// 
// 		glClearColor(background.r, background.g, background.b, background.a);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 		// Вращение камеры:
// 		view = mainCamera.getViewMatrix();
// 
// 		// Поправка на FOV камеры:
// 		projection = glm::perspective(glm::radians(mainCamera.FOV), (float)windowWidth / windowHeight, 0.1f, 100.0f);
// 
// 
// 		// Отрисовка ящиков:
// 
// 		for (auto cub : cubes)
// 		{
// 			cub.draw(view, projection, mainCamera.position, lights);
// 		}
// 
// 		// i = 0;
// 		// for (auto lightCubPos : lightCubePositions) {
// 		// 	model = glm::mat4(1.0f);
// 		// 	model = glm::translate(model, lightCubPos);
// 		// 	// model = glm::rotate(model, glm::radians((float)(90.0f * sin(glfwGetTime()))), glm::vec3(0.1f, 0.2f, 0.3f));
// 		// 	model = glm::scale(model, glm::vec3(lightCubeScales[i], lightCubeScales[i], lightCubeScales[i]));
// 		// 
// 		// 	lightShader->use();
// 		// 	
// 		// 	transformation = projection * view * model;
// 		// 	
// 		// 	lightShader->setFloatMat4("transformation", glm::value_ptr(transformation));
// 		// 	lightShader->setFloatVec3("uColor", lightCubeDiffuse[i]);
// 		// 
// 		// 	glBindVertexArray(VAO_light);
// 		// 	glDrawElements(GL_TRIANGLES, cubeIndicesCount * 3, GL_UNSIGNED_INT, 0);
// 		// 
// 		// 	i++;
// 		// }
// 
// 
// 		glfwSwapBuffers(win);
// 		glfwPollEvents();
// 	}
// 
// 	glfwTerminate();
// 	return 0;
// }
