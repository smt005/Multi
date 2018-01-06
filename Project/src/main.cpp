
#include <windows.h>
#include "Application.h"
#include "Game.h"
#include "Draw/Draw.h"

#define EXAMPLE true

#ifdef WIN32

int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE previousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!Application::createWindow(applicationInstance, Application::_nativeWindow, Application::_deviceContext)) exit(true);
	if (!Application::initGLES()) exit(1);
	
	if (EXAMPLE)
	{
		Draw::nextDraw(4);
	}
	else
	{
		Game::init();
	}

	while (true)
	{
		Application::actionOnFrame();
		
		if (EXAMPLE)
		{
			Draw::draws();
		}
		else
		{
			Game::tact();
		}

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