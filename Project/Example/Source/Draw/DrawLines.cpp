
#include "DrawLines.h"
#include "../Platform/Source/Application.h"
#include "Draw/Shader.h"
#include "Draw/CameraGLM.h"
#include "Draw/DrawEngine.h"
#include "Object/ShapeUnited.h"
#include "Object/Map.h"

#ifdef BUILD_WIN_GLES
#define GL_GLEXT_PROTOTYPES
#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
#include <GL/glew.h>
#elif BUILD_OSX
#include "glfw3.h"
#endif

#ifdef WIN32
#include <fstream>
#include <iostream>
#include <stdlib.h>
#endif

#include <string>
using namespace std;

DrawLines::DrawLines()
{
#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt");
#endif

	/*{
		float v = help::random(0.0f, 10.0f);
		int i = 0;
	}

	{
		float v = help::random(-1.0f, 1.0f);
		int i = 0;
	}

	{
		float v = help::random(-2.0f, -1.0f);
		int i = 0;
	}

	{
		float v = help::random(1.0f, 10.0f);
		int i = 0;
	}*/

    init();
}

DrawLines::~DrawLines()
{
    delete _camera;
    delete _shapeUnited;
}

void DrawLines::init()
{
	RandomLines::create(_lines);
	_lines.check();

    initDraw();
    initCallback();
    
    _map = &Map::getByName("MapGameEvolutionTest");
    
    /*if (!_shapeUnited)
    {
        _shapeUnited = new ShapeUnited();
        _shapeUnited->load("Models/Microbe_view.obj");
    }*/
}

void DrawLines::initDraw()
{
    DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
    DrawEngine::initDrawMap();
	DrawEngine::initDrawLines();
    
    _camera = new CameraGLM();
    _camera->setDefault();
    _camera->setLookAt(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
    _camera->setSpeed(0.1f);
    _camera->setCalcFrustum(false);
}

void DrawLines::initCallback()
{
    this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
    this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
    
    Callback::_hintObject = this;
}

bool DrawLines::closeGame(void *data)
{
    App::close();
    return true;
}

bool DrawLines::rotateCamera(void *data)
{
    CameraGLM::current().rotate(Callback::_vector);
    return true;
}

void DrawLines::draw(bool clear)
{
	DrawEngine::prepareDrawLine();
	
	drawLine(_lines);
}

void DrawLines::drawLine(RandomLines& line)
{
	if (!line._visible)
	{
		return;
	}

	float point0[] = { line._point0.x, line._point0.y, line._point0.z };
	float point1[] = { line._point1.x, line._point1.y, line._point1.z };

	float var = static_cast<float>(line._level + 1) / static_cast<float>(RandomLines::_maxLevel);
	float color[] = { var, var, 0.0, 1.0f };

	DrawEngine::drawLine(point0, point1, color);
	
	for (int i = 0; i < line._count; ++i)
	{
		drawLine(line._lines[i]);
	}
}
