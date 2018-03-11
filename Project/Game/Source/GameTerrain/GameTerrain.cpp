#include "GameTerrain.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "Common/Help.h"

GameTerrain::GameTerrain()
{
}

GameTerrain::~GameTerrain()
{
}

void GameTerrain::init()
{
	initMap();
	initDraw();
	initCallback();
}

void GameTerrain::save()
{
}

void GameTerrain::tact()
{
	_map->action();
}

void GameTerrain::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap(*_map);
}

void GameTerrain::initMap()
{
	_map = &Map::getByName("MapGameTerrain");
}

void GameTerrain::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
	//_camera->setLookAt(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setLookAt(glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameTerrain::initCallback()
{
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
	this->setCallback(EventCallback::BUTTON_UP, UiFunction(pressButton));
	this->setCallback(EventCallback::BUTTON_DOWN, UiFunction(pressButtonDown));
	
	Callback::_hintObject = this;
}

bool GameTerrain::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}

bool GameTerrain::pressButton(void *data)
{
	if (_charButtonUp == 'Q')
	{
		App::close();
		return true;
	}

	if (_charButtonUp == 'A')
	{
		addObject("Cylinder");
		return true;
	}

	return false;
}

bool GameTerrain::pressButtonDown(void *data)
{
	if (_charButtonDown == 'A')
	{
		addObject("Cylinder_025");
		return true;
	}

	return false;
}

void GameTerrain::addObject(char* name)
{
	glm::vec3 randomPos;
	randomPos.x = help::random_f(10.0f, 50.0f);
	randomPos.y = help::random_f(10.0f, 50.0f);
	randomPos.z = help::random_f(1.0f, 10.0f);

#ifdef BUILD_WIN_GLES
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "addObject %s [%f %f %f]\n", name, randomPos.x, randomPos.y, randomPos.z);
#endif

	_map->addObjectToPos(name, 1, randomPos);
}