#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>

using namespace std;

class MouseMovementEvent
	{
	private:
		float x, y;
	public:
		MouseMovementEvent(const float x, const float y): x(x), y(y)
		{
		}

		float getX() const
		{
			return x;
		}

		float getY() const
		{
			return y;
		}
	};

enum class MouseButton
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT = 1,
	MOUSE_MIDDLE = 2
};

class MouseClickEvent
{
private:
	MouseButton clickedButton;
	int x, y;
public:
	MouseClickEvent(const MouseButton clickedButton, const int x, const int y)
		:clickedButton(clickedButton), x(x), y(y)
	{
	}


	MouseButton getClickedButton() const
	{
		return clickedButton;
	}

	int getX() const
	{
		return x;
	}

	int getY() const
	{
		return y;
	}
};

class MouseScrollEvent
{
private:
	float dy;
public:
	explicit MouseScrollEvent(const float dy)
		: dy(dy)
	{
	}

	float getDy() const
	{
		return dy;
	}
};

class KeyboardPressEvent
{
private:
	int keyCode;
	int action;
	int mods;

public:
	KeyboardPressEvent(const int keyCode, const int action, const int mods)
		: keyCode(keyCode),
		  action(action),
		  mods(mods)
	{
	}

	int getKeyCode() const
	{
		return keyCode;
	}

	int getAction() const
	{
		return action;
	}

	int getMods() const
	{
		return mods;
	}
};

class WindowResizeEvent
{
private:
	int width, height;
public:

	WindowResizeEvent(int width, int height)
		: width(width),
		  height(height)
	{
	}


	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}
};

class MouseMovementHandler
{
	// Функция handleFunction должна возвращать успешность обработки события.
	function<bool(const float, const float)> handleFunction;
public:
	MouseMovementHandler(const function<bool(const float, const float)>& handleFunction )
		:handleFunction(handleFunction)
	{
	}
	
	bool handle(MouseMovementEvent event) const
	{
		return handleFunction(event.getX(), event.getX());
	}
};

class MouseClickHandler
{
	// Функция handleFunction должна возвращать успешность обработки события.
	function<bool(MouseButton, const int, const int)> handleFunction;
public:
	MouseClickHandler(const function<bool(MouseButton, const int, const int)>& handleFunction )
		:handleFunction(handleFunction)
	{
	}
	
	bool handle(MouseClickEvent event) const
	{
		return handleFunction(event.getClickedButton(), event.getX(), event.getY());
	}
};

class MouseScrollHandler
{
	// Функция handleFunction должна возвращать успешность обработки события.
	function<bool(const float)> handleFunction;
public:
	MouseScrollHandler(const function<bool(const float)>& handleFunction )
		:handleFunction(handleFunction)
	{
	}
	
	bool handle(MouseScrollEvent event) const
	{
		return handleFunction(event.getDy());
	}
};

class KeyboardPressHandler
{
	// Функция handleFunction должна возвращать успешность обработки события.
	function<bool(const int, const int, const int)> handleFunction;
public:
	KeyboardPressHandler(const function<bool(const int, const int, const int)>& handleFunction )
		:handleFunction(handleFunction)
	{
	}
	
	bool handle(KeyboardPressEvent event) const
	{
		return handleFunction(event.getKeyCode(), event.getAction(), event.getMods());
	}
};

class WindowResizeHandler
{
	// Функция handleFunction должна возвращать успешность обработки события.
	function<bool(const int, const int)> handleFunction;
public:
	WindowResizeHandler(const function<bool(const int, const int)>& handleFunction )
		:handleFunction(handleFunction)
	{
	}
	
	bool handle(WindowResizeEvent event) const
	{
		return handleFunction(event.getWidth(), event.getHeight());
	}
};

class InputHandler
{
private:
	vector<MouseMovementHandler> *mouseMovementHandlers;
	vector<MouseClickHandler> *mouseClickHandlers;
	vector<MouseScrollHandler> *mouseScrollHandlers;
	vector<KeyboardPressHandler> *keyboardPressHandlers;
	vector<WindowResizeHandler> *windowResizeHandlers;

	bool handleMouseMovementEvent(const MouseMovementEvent& event) const
	{		
		bool result = true;
		for (auto && handler : *mouseMovementHandlers)
		{
			result = result && handler.handle(event);
		}
		return result;
	}
	bool handleMouseClickEvent(const MouseClickEvent& event) const
	{
		bool result = true;
		for (auto && handler : *mouseClickHandlers)
		{
			result = result && handler.handle(event);
		}
		return result;
	}
	bool handleMouseScrollEvent(const MouseScrollEvent& event) const
	{
		bool result = true;
		for (auto && handler : *mouseScrollHandlers)
		{
			result = result && handler.handle(event);
		}
		return result;
	}
	bool handleKeyboardPressEvent(const KeyboardPressEvent& event) const
	{
		bool result = true;
		for (auto && handler : *keyboardPressHandlers)
		{
			result = result && handler.handle(event);
		}
		return result;
	}
	bool handleWindowResizeEvent(const WindowResizeEvent& event) const
	{
		bool result = true;
		for (auto && handler : *windowResizeHandlers)
		{
			result = result && handler.handle(event);
		}
		return result;
	}
public:
	InputHandler()
	{
		mouseMovementHandlers = new vector<MouseMovementHandler>();
		mouseClickHandlers = new vector<MouseClickHandler>();
		mouseScrollHandlers = new vector<MouseScrollHandler>();
		keyboardPressHandlers = new vector<KeyboardPressHandler>();
		windowResizeHandlers = new vector<WindowResizeHandler>();
	}

	~InputHandler()
	{
		delete mouseMovementHandlers;
	}

	bool handle(const MouseMovementEvent& event)
	{
		return handleMouseMovementEvent(event);
	}
	bool handle(const MouseClickEvent& event)
	{
		return handleMouseClickEvent(event);
	}
	bool handle(const MouseScrollEvent& event)
	{
		return handleMouseScrollEvent(event);
	}
	bool handle(const KeyboardPressEvent& event)
	{
		return handleKeyboardPressEvent(event);
	}
	bool handle(const WindowResizeEvent& event)
	{
		return handleWindowResizeEvent(event);
	}
	
	void addMouseMovementHandler(const MouseMovementHandler& handler)
	{
		mouseMovementHandlers->push_back(handler);
	}

	// void removeMouseMovementHandler(const MouseMovementHandler& handler)
	// {
	// 	// Удаление из вектора по значению.
	// 	mouseMovementHandlers->erase(remove(mouseMovementHandlers->begin(), mouseMovementHandlers->end(), handler), mouseMovementHandlers->end());
	// }

	void addMouseClickHandler(const MouseClickHandler& handler)
	{
		mouseClickHandlers->push_back(handler);
	}

	// void removeMouseClickHandler(const MouseClickHandler& handler)
	// {
	// 	// Удаление из вектора по значению.
	// 	mouseClickHandlers->erase(remove(mouseClickHandlers->begin(), mouseClickHandlers->end(), handler), mouseClickHandlers->end());
	// }

	void addMouseScrollHandler(const MouseScrollHandler& handler)
	{
		mouseScrollHandlers->push_back(handler);
	}

	// void removeMouseScrollHandler(const MouseScrollHandler& handler)
	// {
	// 	// Удаление из вектора по значению.
	// 	mouseScrollHandlers->erase(remove(mouseScrollHandlers->begin(), mouseScrollHandlers->end(), handler), mouseScrollHandlers->end());
	// }

	void addKeyboardPressHandler(const KeyboardPressHandler& handler)
	{
		keyboardPressHandlers->push_back(handler);
	}

	// void removeKeyboardPressHandler(const KeyboardPressHandler& handler)
	// {
	// 	// Удаление из вектора по значению.
	// 	keyboardPressHandlers->erase(remove(keyboardPressHandlers->begin(), keyboardPressHandlers->end(), handler), keyboardPressHandlers->end());
	// }

	void addWindowResizeHandler(const WindowResizeHandler& handler)
	{
		windowResizeHandlers->push_back(handler);
	}

	// void removeWindowResizeHandler(const WindowResizeHandler& handler)
	// {
	// 	// Удаление из вектора по значению.
	// 	windowResizeHandlers->erase(remove(windowResizeHandlers->begin(), windowResizeHandlers->end(), handler), windowResizeHandlers->end());
	// }
};

// int main(int argc, char* argv[])
// {
// 	const function<bool(const int, const int)> printCoordinatesLambda = [](const int x, const int y)
// 	{
// 		cout << "Mouse moved! x=" << x << " y=" << y << "\n";
// 		return true;
// 	};
// 	
// 	InputHandler ih;
// 	MouseMovementHandler mmh(printCoordinatesLambda);
// 	ih.addMouseMovementHandler(mmh);
// 	MouseMovementEvent mme(10, -10);
// 	ih.handle(mme);
// }
