#pragma once

#include <windows.h>
#include "EGL/egl.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

class ApplicationPlatform
{
public:
	static HDC _deviceContext;
	static HWND _nativeWindow;
	static EGLDisplay _eglDisplay;
	static EGLConfig _eglConfig;
	static EGLSurface _eglSurface;
	static EGLContext _eglContext;

	static int WindowWidth;
	static int WindowHeight;

public:
	static bool execution(HINSTANCE& applicationInstance);
	static bool createWindow(HINSTANCE applicationInstance, HWND& nativeWindow, HDC& deviceContext);
	static bool initGLES();

public:
	static void actionOnFrame();

	static int width();
	static int height();
	static float aspect();
	static double getCurentTime();
	static void close();

private:
	static bool createEGLDisplay();
	static bool chooseEGLConfig();
	static bool createEGLSurface();
	static bool setupEGLContext();
	static bool testEGLError(const char* functionLastCalled);
	static void releaseEGLState();
};
