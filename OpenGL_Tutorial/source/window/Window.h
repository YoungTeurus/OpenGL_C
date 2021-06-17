#pragma once

#include <glad\glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "InputHandler.h"

class Window
{
private:
	static Window* instance;

	const int initialWidth = 1024;
	const int initialHeight = 720;
	const std::string initialTitle = "OpenGL Tutorial";

	int width = initialWidth;
	int height = initialHeight;

	std::string title = initialTitle;

	GLFWwindow *win;
	
	InputHandler inputHandler;
	
	bool initializeAndReturnSuccess();
	void setWindowHints();
	void setCallbacks();

	Window()
	{
		const bool initializationResult = initializeAndReturnSuccess();
		if (initializationResult)
		{
			cout << "Window was successfully initialized!";
		} else
		{
			cout << "An error happened during window initializing!";
		}
	}
public:
	static Window* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Window();
		}
		return instance;
	}

	InputHandler getInputHandler() const
	{
		return inputHandler;
	}

	bool isWindowRunning() const
	{
		return !glfwWindowShouldClose(win);
	}

	void swapBuffer() const
	{
		glfwSwapBuffers(win);
	}

	void pollEvents() const
	{
		glfwPollEvents();
	}

	void terminateWindow() const
	{
		glfwTerminate();
	}

	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}

	std::string getTitle() const
	{
		return title;
	}

	void setTitle(const std::string& newTitle)
	{
		title = newTitle;
		glfwSetWindowTitle(win, title.c_str());
	}
};
