#include "GameMini.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "AI/AIExample.h"

#include "Object/Map.h"
#include "Object/Object.h"

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

void GameMini::save()
{
	json dataJson;

	if (_map)
	{
		_map->getDataJson(dataJson);
	}

	std::string dataString = dataJson.dump();
	FilesManager::saveTextFile("Saves/GameMini.sav", dataString.c_str());
}

void GameMini::tact()
{
	if (_timerTarget > 300)
	{
		_timerTarget = 0.0;
		if (_map)
		{
			int diameter = static_cast<int>(_map->getArea());
			int radius = static_cast<int>(_map->getArea() * 0.5f);
			float k = 0.5f;
			float posX = static_cast<float>((rand() % diameter + 1) - radius) * k;
			float posY = static_cast<float>((rand() % diameter + 1) - radius) * k;
			float posZ = 1.0f;
			glm::mat4x4 matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(posX, posY, posZ));

			Object& target = _map->_objects.getByName("Target");
			target.setMatrix(matrix);
		}
	}
	++_timerTarget;

	_map->action();
}

void GameMini::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap(*_map);
}

void GameMini::initMap()
{
	_map = &Map::getByName("Map");
	AIExample::findTarget(*_map, "Target");
}

void GameMini::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
    _camera->setLookAt(glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameMini::initCallback()
{
	this->setCallback(EventCallback::TAP_DOUBLE, Function(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, Function(rotateCamera));

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
