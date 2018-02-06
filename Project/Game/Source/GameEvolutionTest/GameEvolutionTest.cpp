#include "GameEvolutionTest.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "AI/AIExample.h"

GameEvolutionTest::GameEvolutionTest()
{
}

GameEvolutionTest::~GameEvolutionTest()
{
}

void GameEvolutionTest::init()
{
	initMap();
	initDraw();
	initCallback();
	physics.createWorldTest();
}

void GameEvolutionTest::save()
{
	json dataJson;

	if (_map)
	{
		_map->getDataJson(dataJson);
	}

	std::string dataString = dataJson.dump();
	FilesManager::saveTextFile("Saves/GameEvolutionTest.sav", dataString.c_str());
}

void GameEvolutionTest::tact()
{
	physics.update();

	if (_timerTarget > 300)
	{
		_timerTarget = 0.0;
		if (_map)
		{
			glm::mat4x4 matrix = glm::translate(glm::mat4x4(1.0f), randomPos());

			Object &target = _map->_objects.getByName("Target");
			target.setMatrix(matrix);
		}
	}
	++_timerTarget;

	_map->action();
}

void GameEvolutionTest::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap(*_map);
}

void GameEvolutionTest::initMap()
{
	_map = &Map::getByName("MapGameEvolutionTest");
	AIExample::findTarget(*_map, "Target");
}

void GameEvolutionTest::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
    _camera->setLookAt(glm::vec3(-15.0f, -15.0f, 15.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameEvolutionTest::initCallback()
{
	this->setCallback(EventCallback::TAP_LONG, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
	this->setCallback(EventCallback::TAP_UP, UiFunction(addObject));
	
	Callback::_hintObject = this;
}

glm::vec3 GameEvolutionTest::randomPos(const float& k, const int& zBottom, const int& zTop)
{
	int diameter = static_cast<int>(_map->getArea());
	int radius = static_cast<int>(_map->getArea() * 0.5f);
	
	float z = zBottom;
	if (zBottom != zTop)
	{
		int h = zTop - zBottom;
		z = static_cast<float>(zBottom) + static_cast<float>(rand() % h + 1);
	}

	return glm::vec3(static_cast<float>((rand() % diameter + 1) - radius) * k, static_cast<float>((rand() % diameter + 1) - radius) * k, z);
}

bool GameEvolutionTest::closeGame(void *data)
{
	App::close();
	return true;
}

bool GameEvolutionTest::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}

bool  GameEvolutionTest::addObject(void *data)
{
	_map->addObjectToPos("Box1", 1, glm::vec3(randomPos(0.01, 2, 20)));
	return true;
}
