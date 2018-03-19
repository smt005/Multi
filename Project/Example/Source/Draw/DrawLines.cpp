
#include "DrawLines.h"
#include "../Platform/Source/Application.h"
#include "Draw/Shader.h"
#include "Draw/CameraGLM.h"
#include "Draw/DrawEngine.h"
#include "Object/Shape.h"
#include "Object/Map.h"

#ifdef BUILD_WIN_GLES
#define GL_GLEXT_PROTOTYPES
#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
#include <GL/glew.h>
#elif BUILD_OSX
#include "glfw3.h"
#endif

#include <string>
using namespace std;

DrawLines::DrawLines()
{
    init();
}

DrawLines::~DrawLines()
{
    delete _camera;
}

void DrawLines::init()
{
	RandomLines::create(_lines);
	_lines.check();

    initDraw();
    initCallback();
    
    _map = &Map::getByName("MapGameEvolutionTest");
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
    this->setCallback(EventCallback::TAP_DOUBLE, Function(closeGame));
    this->setCallback(EventCallback::TAP_PINCH, Function(rotateCamera));
    
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
