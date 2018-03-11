#include "GameTerrain.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"

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
	//this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
	this->setCallback(EventCallback::BUTTON_UP, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));

	Callback::_hintObject = this;
}

bool GameTerrain::closeGame(void *data)
{
	if (_charButtonUp != 'Q') return false;

	App::close();
	return true;
}

bool GameTerrain::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}
