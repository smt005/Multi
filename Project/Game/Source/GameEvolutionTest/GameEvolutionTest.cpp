#include "GameEvolutionTest.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "AI/AIExample.h"

#include "Object/Object.h"
#include "Object/Model.h"
#include "Object/Shape.h"

#include "Common/Help.h"

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
	Physics::init();
}

void GameEvolutionTest::save()
{
	json dataJson;

	if (_map)
	{
		_map->getDataJson(dataJson);
	}
}

void GameEvolutionTest::tact()
{
	Physics::update();

	if (_timerTarget > 300)
	{
		_timerTarget = 0.0;
		if (_map)
		{
			glm::mat4x4 matrix = glm::translate(glm::mat4x4(1.0f), randomPos());

			Object& target = help::find(_map->_objects, "Target");
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
	_map = Map::getByName("MapGameEvolutionTest");
	AIExample::findTarget(*_map, "Target");
}

void GameEvolutionTest::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	CameraGLM::current.setDefault();
	CameraGLM::current.setLookAt(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	CameraGLM::current.setSpeed(0.1f);
	CameraGLM::current.setCalcFrustum(false);
}

void GameEvolutionTest::initCallback()
{
	this->setCallback(EventCallback::TAP_DOUBLE, Function(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, Function(rotateCamera));
	
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
	CameraGLM::current.rotate(Callback::_vector);
	return true;
}

bool  GameEvolutionTest::addObject(void *data)
{
	auto create = [this]() {
		_map->addObjectToPos("Box1", PhysicType::CONVEX, glm::vec3(randomPos(0.91, 29, 50)));
		_map->addObjectToPos("Sphere_005", PhysicType::CONVEX, glm::vec3(randomPos(0.91, 29, 50)));
		_map->addObjectToPos("Cylinder_02", PhysicType::CONVEX, glm::vec3(randomPos(0.91, 29, 50)));

		_countObjects += 4;
		return true;
	};

	if (_countObjects < 100)
	{
		int increase = 100;

		for (int i = 0; i < increase; ++i)
		{
			create();
		}
	}

	return true;
}
