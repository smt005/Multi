
#include <windows.h>
#include "Game.h"
#include "Draw/Draw.h"
#include "ApplicationPlatform.h"

#define EXAMPLE false

#ifdef WIN32

int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE previousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!ApplicationPlatform::createWindow(applicationInstance, ApplicationPlatform::_nativeWindow, ApplicationPlatform::_deviceContext)) exit(true);
	if (!ApplicationPlatform::initGLES()) exit(1);
	
	if (EXAMPLE)
	{
		Draw::nextDraw(4);
	}
	else
	{
		Game::gameInit();
	}

	while (true)
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

		if (!eglSwapBuffers(ApplicationPlatform::_eglDisplay, ApplicationPlatform::_eglSurface)) return 0;

		// Check for messages from the windowing system. These will pass through the callback registered earlier.
		MSG eventMessage;
		PeekMessage(&eventMessage, ApplicationPlatform::_nativeWindow, NULL, NULL, PM_REMOVE);
		TranslateMessage(&eventMessage);
		DispatchMessage(&eventMessage);
	}

	return 0;
}

#else OSX
	// OSX
#endif