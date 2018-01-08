#include "GameMini.h"
#include "Application.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"

GameMini::GameMini()
{
}

GameMini::~GameMini()
{
}

void GameMini::init()
{
	initMap();
	initDraw();
	initCallback();
}

void GameMini::tact()
{
	_map->action();
}

void GameMini::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap();
}

void GameMini::initMap()
{
	_map = &Map::getByName("Map");

	// Добавление глайдеров
	
}

void GameMini::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
	_camera->setLookAt(glm::vec3(3.0f, 1.5f, 3.0f), glm::vec3(.5f, .25f, .0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameMini::initCallback()
{
	this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));

	Callback::_hintObject = this;
}

bool GameMini::closeGame(void *data)
{
	App::close();
	return true;
}

bool GameMini::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}
