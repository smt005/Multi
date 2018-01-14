
#include "Draw.h"
#include "DrawHelloTriangle.h"
#include "DrawGreed.h"
#include "DrawLoadOBJ.h"
#include "DrawMap.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
	#include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

Draw::Draw()
{
	_status = false;
}

Draw::~Draw()
{

}

void Draw::draw(bool clear)
{
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int _classWindow = 0;
Draw* _currentDraw = nullptr;

void Draw::resetNumber()
{
	_classWindow = 0;
	nextDraw(1);
}

void Draw::draws()
{
	if (!_currentDraw)
	{
		glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	_currentDraw->draw();
}

bool Draw::nextDraw(int step)
{
	_classWindow = _classWindow + step;

	if (_classWindow < 0) _classWindow = 0;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	switch (_classWindow)
	{
	case 0:
	{
		delete _currentDraw;
		_currentDraw = 0;
	} break;
	case 1:
	{
		delete _currentDraw;
		_currentDraw = new DrawHelloTriangle();
	} break;

	case 2:
	{
		delete _currentDraw;
		_currentDraw = new DrawGreed();
	} break;
	
	case 3:
	{
		delete _currentDraw;
		_currentDraw = new DrawLoadOBJ();
	} break;
	
	case 4:
	{
		delete _currentDraw;
		_currentDraw = new DrawMap();
	} break;

	default: return false;
	}

	return true;
}
