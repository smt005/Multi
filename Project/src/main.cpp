
#include <windows.h>
#include "EGL/egl.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

HDC _deviceContext = nullptr;
HWND _nativeWindow = nullptr;
EGLDisplay _eglDisplay = nullptr;
EGLConfig _eglConfig = nullptr;
EGLSurface _eglSurface = nullptr;
EGLContext _eglContext = nullptr;

bool createEGLDisplay();
bool chooseEGLConfig();
bool createEGLSurface();
bool setupEGLContext();
bool testEGLError(const char* functionLastCalled);
void releaseEGLState();

bool init()
{
	if (!createEGLDisplay())
	{
		goto release;
	}

	if (!chooseEGLConfig())
	{
		goto release;
	}

	if (!createEGLSurface())
	{
		goto release;
	}

	if (!setupEGLContext())
	{
		goto release;
	}

	return true;

release:
	eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(_eglDisplay);

	// Release the device context.
	if (_deviceContext)
	{
		ReleaseDC(_nativeWindow, _deviceContext);
	}

	// Destroy the window
	if (_nativeWindow)
	{
		DestroyWindow(_nativeWindow);
	}

	return false;
}


bool createEGLDisplay()
{
	_eglDisplay = eglGetDisplay(_deviceContext);
	if (_eglDisplay == EGL_NO_DISPLAY)
	{
		_eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	}

	// If a display still couldn't be obtained, return an error.
	if (_eglDisplay == EGL_NO_DISPLAY)
	{
		MessageBox(0, "Failed to get an EGLDisplay", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	EGLint eglMajorVersion, eglMinorVersion;
	if (!eglInitialize(_eglDisplay, &eglMajorVersion, &eglMinorVersion))
	{
		MessageBox(0, "Failed to initialize the EGLDisplay", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

bool chooseEGLConfig()
{
	const EGLint configurationAttributes[] =
	{
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};

	EGLint configsReturned;
	if (!eglChooseConfig(_eglDisplay, configurationAttributes, &_eglConfig, 1, &configsReturned) || (configsReturned != 1))
	{
		MessageBox(0, "eglChooseConfig() failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

bool createEGLSurface()
{
	_eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, _nativeWindow, NULL);
	if (_eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		_eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, NULL, NULL);
	}

	// Check for any EGL Errors
	if (!testEGLError("eglCreateWindowSurface"))
	{
		return false;
	}
	return true;
}

bool setupEGLContext()
{
	eglBindAPI(EGL_OPENGL_ES_API);
	if (!testEGLError("eglBindAPI"))
	{
		return false;
	}

	EGLint contextAttributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	_eglContext = eglCreateContext(_eglDisplay, _eglConfig, NULL, contextAttributes);
	if (!testEGLError("eglCreateContext"))
	{
		return false;
	}

	eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext);
	if (!testEGLError("eglMakeCurrent"))
	{
		return false;
	}

	return true;
}

bool testEGLError(const char* functionLastCalled)
{
	EGLint lastError = eglGetError();
	if (lastError != EGL_SUCCESS)
	{
		TCHAR stringBuffer[256];
		MessageBox(_nativeWindow, stringBuffer, "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

const int WindowWidth = 1500;
const int WindowHeight = 1000;

#define	WINDOW_CLASS_NAME	"ExampleClass"
#define APPLICATION_NAME	"Example OpenGL ES 2 v.0.0 [" __DATE__"  " __TIME__" ]"

LRESULT CALLBACK handleWindowMessages(HWND nativeWindow, UINT message, WPARAM windowParameters, LPARAM longWindowParameters);

bool createWindow(HINSTANCE applicationInstance, HWND& nativeWindow, HDC& deviceContext)
{
	// Describe the native window in a window class structure
	WNDCLASS nativeWindowDescription;
	nativeWindowDescription.style = CS_HREDRAW | CS_VREDRAW;
	nativeWindowDescription.lpfnWndProc = handleWindowMessages;
	nativeWindowDescription.cbClsExtra = 0;
	nativeWindowDescription.cbWndExtra = 0;
	nativeWindowDescription.hInstance = applicationInstance;
	nativeWindowDescription.hIcon = 0;
	nativeWindowDescription.hCursor = 0;
	nativeWindowDescription.lpszMenuName = 0;
	nativeWindowDescription.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	nativeWindowDescription.lpszClassName = WINDOW_CLASS_NAME;

	// Register the windows class with the OS.
	ATOM registerClass = RegisterClass(&nativeWindowDescription);
	if (!registerClass)
	{
		MessageBox(0, "Failed to register the window class", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Create a rectangle describing the area of the window
	RECT windowRectangle;
	SetRect(&windowRectangle, 0, 0, WindowWidth, WindowHeight);
	AdjustWindowRectEx(&windowRectangle, WS_CAPTION | WS_SYSMENU, false, 0);

	// Create the window from the available information
	nativeWindow = CreateWindow(WINDOW_CLASS_NAME, APPLICATION_NAME, WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, NULL, NULL, applicationInstance, NULL);
	//nativeWindow = CreateWindow(WINDOW_CLASS_NAME, APPLICATION_NAME, WS_VISIBLE | WS_SYSMENU, 0, 0, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, NULL, NULL, applicationInstance, NULL);
	if (!nativeWindow)
	{
		MessageBox(0, "Failed to create the window", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Get the associated device context from the window
	deviceContext = GetDC(nativeWindow);
	if (!deviceContext)
	{
		MessageBox(nativeWindow, "Failed to create the device context", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE previousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!createWindow(applicationInstance, _nativeWindow, _deviceContext)) exit(true);
	if (!init()) exit(true);


	while (true)
	{
		glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		if (!eglSwapBuffers(_eglDisplay, _eglSurface)) return 0;

		// Check for messages from the windowing system. These will pass through the callback registered earlier.
		MSG eventMessage;
		PeekMessage(&eventMessage, _nativeWindow, NULL, NULL, PM_REMOVE);
		TranslateMessage(&eventMessage);
		DispatchMessage(&eventMessage);
	}

	return 0;
}

LRESULT CALLBACK handleWindowMessages(HWND nativeWindow, UINT message, WPARAM windowParameters, LPARAM longWindowParameters)
{
	switch (message)
	{

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			exit(true);
			return 1;
		} break;

	}

	return DefWindowProc(nativeWindow, message, windowParameters, longWindowParameters);
}