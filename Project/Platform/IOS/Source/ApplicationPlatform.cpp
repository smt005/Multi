
#include "Game/Source/Game.h"
#include "Example/Source/Draw/Draw.h"
#include "ApplicationPlatform.h"
#include "Engine/Source/Callback/Callback.h"
#include <iostream>
#include <sys/time.h>

#define ApplicationWin_NAME    "Multi v.0.0 [" __DATE__"  " __TIME__" ]"

#define EXAMPLE false

int ApplicationPlatform::_windowWidth = 640;
int ApplicationPlatform::_windowHeight = 480;
float ApplicationPlatform::_cursorPos[2];

bool ApplicationPlatform::execution()
{
    return true;
}

void ApplicationPlatform::actionOnFrame()
{
    Callback::move(_cursorPos);
    Callback::tap_pinch();
}

int ApplicationPlatform::width()
{
    int width = _windowWidth;
    // ...
    return width != 0 ? width : 1;
}

int ApplicationPlatform::height()
{
    int height = _windowHeight;
    // ...
    return height != 0 ? height : 1;
}

float ApplicationPlatform::aspect()
{
    int width = _windowWidth;
    int height = _windowHeight;
    
    // ...
    
    return height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
}

double ApplicationPlatform::getCurentTime()
{
    timeval time;
    gettimeofday(&time, NULL);
    long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return static_cast<double>(millis);
    
    return 0;
}

void ApplicationPlatform::close()
{
    exit(1);
}

//-----------------

