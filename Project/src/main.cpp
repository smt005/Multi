
#include <windows.h>
#include "Application.h"
#include "Draw/Draw.h"

#ifdef WIN32

int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE previousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!Application::createWindow(applicationInstance, Application::_nativeWindow, Application::_deviceContext)) exit(true);
	if (!Application::initGLES()) exit(true);
	
	Draw::nextDraw(4);

	while (true)
	{
		Draw::draws();

		if (!eglSwapBuffers(Application::_eglDisplay, Application::_eglSurface)) return 0;

		// Check for messages from the windowing system. These will pass through the callback registered earlier.
		MSG eventMessage;
		PeekMessage(&eventMessage, Application::_nativeWindow, NULL, NULL, PM_REMOVE);
		TranslateMessage(&eventMessage);
		DispatchMessage(&eventMessage);
	}

	return 0;
}
#else OSX
	// OSX
#endif