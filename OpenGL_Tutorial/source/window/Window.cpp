#include "Window.h"

#include <glad/glad.h>
#include <iostream>

bool Window::initializeAndReturnSuccess()
{
	// Настройка glfw
	glfwInit();
	setWindowHints();

	win = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (win == NULL)
	{
		std::cout << "Error while creating the window.";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(win);

	// Получение функций видеокарты
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "Error while loading GLAD.";
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Захватываем курсор

	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	return true;
}

void Window::setWindowHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);  // Анти-алиасинг
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window *win = Window::getInstance();
	win->getInputHandler().handle(
		MouseMovementEvent(xpos, ypos)
	);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	Window *win = Window::getInstance();
	win->getInputHandler().handle(
		MouseScrollEvent((float)yOffset)
	);
}

void onKeyCallback(GLFWwindow* window, int keyCode, int scancode, int action, int mods)
{
	Window *win = Window::getInstance();
	win->getInputHandler().handle(
		KeyboardPressEvent(keyCode, action, mods)
	);
}

void onResizeCallback(GLFWwindow* window, int width, int height)
{
	Window *win = Window::getInstance();
	win->getInputHandler().handle(
		WindowResizeEvent(width, height)
	);
}

void Window::setCallbacks()
{
	glfwSetCursorPosCallback(win, mouseCallback);
	glfwSetScrollCallback(win, scrollCallback);
	glfwSetKeyCallback(win, onKeyCallback);
	glfwSetFramebufferSizeCallback(win, onResizeCallback);

	const function<bool(const float, const float)> simpleMovementHandlerFunc = [](const float xpos, const float ypos)
	{
		cout << "Mouse moved! xpos=" << xpos << " ypos" << ypos << "\n";
		return true;
	};
	inputHandler.addMouseMovementHandler( MouseMovementHandler( simpleMovementHandlerFunc ) );

	const function<bool(const float)> simpleScrollHandlerFunc = [](const float yOffset)
	{
		cout << "Mouse scrolled! yOffset=" << yOffset << "\n";
		return true;
	};
	inputHandler.addMouseScrollHandler( MouseScrollHandler( simpleScrollHandlerFunc ) );

	const function<bool(const int, const int, const int)> simpleKeyboardPressHandlerFunc = [](const int keyCode, const int action, const int mods)
	{
		cout << "Key pressed! keyCode=" << keyCode << " action=" << action << " mods=" << mods << "\n";
		return true;
	};
	inputHandler.addKeyboardPressHandler( KeyboardPressHandler( simpleKeyboardPressHandlerFunc ) );

	const function<bool(const int, const int)> simpleWindowResizeHandlerFunc = [](const int width, const int height)
	{
		cout << "Window resized! width=" << width << " height=" << height << "\n";
		return true;
	};
	inputHandler.addWindowResizeHandler( WindowResizeHandler( simpleWindowResizeHandlerFunc ) );
}
