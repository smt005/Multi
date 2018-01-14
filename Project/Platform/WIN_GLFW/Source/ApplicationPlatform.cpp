#include "Game/Source/Game.h"
#include "Example/Source/Draw/Draw.h"
#include "ApplicationPlatform.h"
#include "Engine/Source/Callback/Callback.h"

#include <iostream>

#define	WINDOW_CLASS_NAME	"MultiClass"
#define ApplicationWin_NAME	"Multi v.0.0 [" __DATE__"  " __TIME__" ]"

#define EXAMPLE false

int ApplicationPlatform::WindowWidth = 1500;
int ApplicationPlatform::WindowHeight = 1000;

bool ApplicationPlatform::execution()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WindowWidth, WindowHeight, ApplicationWin_NAME, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwSetMouseButtonCallback(window, ApplicationPlatform::mouseButtonCallback);
	glfwSetCursorPosCallback(window, ApplicationPlatform::cursorPositionCallback);
	glfwSetKeyCallback(window, ApplicationPlatform::keyCallback);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	if (EXAMPLE)
	{
		Draw::nextDraw(4);
	}
	else
	{
		Game::gameInit();
	}

	while (!glfwWindowShouldClose(window))
	{
		ApplicationPlatform::actionOnFrame();

		if (EXAMPLE)
		{
			Draw::draws();
		}
		else
		{
			Game::gameTact();
			Game::gameDraw();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return true;
}

void ApplicationPlatform::actionOnFrame()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	float pos[] = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
	Callback::move(pos);
	Callback::tap_pinch();
}

int ApplicationPlatform::width()
{
	int width = WindowWidth;
	// ...
	return width != 0 ? width : 1;
}

int ApplicationPlatform::height()
{
	int height = WindowHeight;
	// ...
	return height != 0 ? height : 1;
}

float ApplicationPlatform::aspect()
{
	int width = WindowWidth;
	int height = WindowHeight;

	// ...

	return height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
}

double ApplicationPlatform::getCurentTime()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	int currentTime = (systemTime.wMinute * 60000) + (systemTime.wSecond * 1000) + systemTime.wMilliseconds;
	return static_cast<double>(currentTime);
}

void ApplicationPlatform::close()
{
	exit(1);
}

//-----------------

void ApplicationPlatform::cursorPositionCallback(GLFWwindow* Window, double x, double y)
{

}

void ApplicationPlatform::mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	switch (Action)
	{
	case GLFW_PRESS:
	{
		switch (Button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			Callback::tap_down();
		}
		break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{

		}
		break;
		case GLFW_MOUSE_BUTTON_RIGHT:
		{

		}
		break;
		}
	}
	break;

	case GLFW_RELEASE:
	{
		switch (Button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			Callback::tap_up();
		}
		break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{

		}
		break;
		case GLFW_MOUSE_BUTTON_RIGHT:
		{

		}
		break;
		}
	}
	break;
	}
}

void ApplicationPlatform::keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{

}
