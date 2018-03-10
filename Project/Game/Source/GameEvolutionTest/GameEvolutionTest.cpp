#include "GameEvolutionTest.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "AI/AIExample.h"

#include "Object/Shape.h"
#include "Object/ShapeUnited.h"

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

	//std::string dataString = dataJson.dump();
	//FilesManager::saveTextFile("Saves/GameEvolutionTest.sav", dataString.c_str());
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

			ObjectUnited &target = _map->_objects.getByName("Target");
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

	if (shapeUnited)
	{
		DrawEngine::drawMesh(*shapeUnited);
	}
}

void GameEvolutionTest::initMap()
{

	if (!shapeUnited)
	{
		shapeUnited = new ShapeUnited();
		//shapeUnited->load("Models/Microbe_view.obj");
		//shapeUnited->load("Models/Terrain.obj");
		//shapeUnited->load("Models/Terrain_box.obj");
		//shapeUnited->load("Models/Terrain_box_mono.obj");
		shapeUnited->load("Models/Box1.obj");
	}

	_map = &Map::getByName("MapGameEvolutionTest");
	AIExample::findTarget(*_map, "Target");
}

void GameEvolutionTest::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::initDrawMap();

	_camera = &CameraGLM::getByName("First");
	_camera->setDefault();
	_camera->setLookAt(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(0.5f, 0.5f, 1.0f));
    //_camera->setLookAt(glm::vec3(-15.0f, -15.0f, 15.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	//_camera->setLookAt(glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	_camera->setSpeed(0.1f);
	_camera->setCalcFrustum(false);
}

void GameEvolutionTest::initCallback()
{
	//this->setCallback(EventCallback::TAP_LONG, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));
	//this->setCallback(EventCallback::TAP_UP, UiFunction(addObject));
	
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
	auto create = [this]() {
		_map->addObjectBoxToPos("Box1", _idShape, 1, glm::vec3(randomPos(0.91, 29, 50)));
		_map->addObjectToPos("Box1", 1, glm::vec3(randomPos(0.91, 29, 50)));
		_map->addObjectToPos("Sphere_005", 1, glm::vec3(randomPos(0.91, 29, 50)));
		_map->addObjectToPos("Cylinder_02", 1, glm::vec3(randomPos(0.91, 29, 50)));

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
