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
	Physics::init();
	//Physics::createWorldTest();

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
	Physics::update();
}

void GameTerrain::draw()
{
	DrawEngine::prepareDraw(true);

	if (_visiblePhysic)
	{
		DrawEngine::drawMap(*_map);
	}
	else
	{
		DrawEngine::drawMapPhysic(*_map);
	}
}

void GameTerrain::initMap()
{
	_map = &Map::getByName("MapGameTerrain");
	_map->setPhysic();
}

void GameTerrain::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
	_camera->setLookAt(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameTerrain::initCallback()
{
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
	this->setCallback(EventCallback::BUTTON_UP, UiFunction(pressButton));
	this->setCallback(EventCallback::BUTTON_PINCH, UiFunction(pressButtonPinch));
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

	if (_charButtonUp == 'E')
	{
		addObject("Cylinder");
		return true;
	}

	return false;
}

bool GameTerrain::pressButtonPinch(void *data)
{
	if (Callback::_key['W'])
	{
		CameraGLM::current().move(CAMERA_FORVARD, 1.0f);
	}

	if (Callback::_key['S'])
	{
		CameraGLM::current().move(CAMERA_BACK, 1.0f);
	}

	if (Callback::_key['A'])
	{
		CameraGLM::current().move(CAMERA_RIGHT, 1.0f);
	}

	if (Callback::_key['D'])
	{
		CameraGLM::current().move(CAMERA_LEFT, 1.0f);
	}

	if (Callback::_key['R'])
	{
		CameraGLM::current().move(CAMERA_TOP, 1.0f);
	}

	if (Callback::_key['F'])
	{
		CameraGLM::current().move(CAMERA_DOWN, 1.0f);
	}

	return true;
}

bool GameTerrain::pressButtonDown(void *data)
{
	if (_charButtonDown == 'E')
	{
		addObject("Chain");
		return true;
	}


	if (Callback::_key['Z'] && Callback::_key['P'])
	{
		_visiblePhysic = !_visiblePhysic;
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

	_map->addObjectToPos(name, PhysicType::CONVEX, randomPos);
}