#pragma once

#include "glfw3.h"

class ApplicationPlatform
{
public:
    enum mouse_button
    {
        MOUSE_BUTTON_NONE = 0,
        MOUSE_BUTTON_LEFT = (1 << 0),
        MOUSE_BUTTON_RIGHT = (1 << 1),
        MOUSE_BUTTON_MIDDLE = (1 << 2)
    };
    
    enum key_action
    {
        KEY_PRESS = GLFW_PRESS,
        KEY_RELEASE = GLFW_RELEASE,
        KEY_REPEAT = GLFW_REPEAT
    };

public:
    static bool execution();
    
public:
	static void actionOnFrame();

	static int width();
	static int height();
	static float aspect();
	static double getCurentTime();
	static void close();

private:
    static int _windowWidth;
    static int _windowHeight;
    static float _cursorPos[2];
    
    static void cursorPositionCallback(GLFWwindow* Window, double x, double y);
    static void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
    static void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
};
