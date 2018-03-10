#include "GameTerrain.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "Object/ShapeUnited.h"
#include "Object/ModelUnited.h"
#include "Object/ObjectUnited.h"

GameTerrain::GameTerrain()
{
}

GameTerrain::~GameTerrain()
{
	delete _shapeUnited;
	_shapeUnited = nullptr;
}

void GameTerrain::init()
{
	initMap();
	initDraw();
	initCallback();
}

void GameTerrain::save()
{
	json dataJson;

	if (_map)
	{
		_map->getDataJson(dataJson);
	}
}

void GameTerrain::tact()
{
	_map->action();
}

void GameTerrain::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap(*_map);

	/*if (_shapeUnited)
	{
		DrawEngine::drawMesh(*_shapeUnited);
	}*/

	/*if (_modelUnited)
	{
		glm::mat4x4 mat(1.0f);
		mat = glm::translate(mat, glm::vec3(0.f, 0.0f, 2.0f));
		DrawEngine::drawModel(*_modelUnited, glm::value_ptr(mat));
	}*/

	/*if (_objectUnited)
	{
		DrawEngine::drawObject(*_objectUnited);
	}*/
}

void GameTerrain::initMap()
{
	_map = &Map::getByName("MapGameTerrain");

	/*if (!_shapeUnited)
	{
		_shapeUnited = new ShapeUnited();
		_shapeUnited->load("Models/Glider.obj");
	}*/

	/*if (!_modelUnited)
	{
		_modelUnited = &ModelUnited::getByName("First");
	}*/

	/*if (!_objectUnited)
	{
		//object.set(name, modelName, pos);
		
		_objectUnited = new ObjectUnited();
		vec3 pos = vec3(-2.0f, -2.0f, 3.0f);
		_objectUnited->set("First", "First", pos);
	}*/
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
	this->setCallback(EventCallback::TAP_DOUBLE, UiFunction(closeGame));
	this->setCallback(EventCallback::TAP_PINCH, UiFunction(rotateCamera));

	Callback::_hintObject = this;
}

bool GameTerrain::closeGame(void *data)
{
	App::close();
	return true;
}

bool GameTerrain::rotateCamera(void *data)
{
	CameraGLM::current().rotate(Callback::_vector);
	return true;
}
