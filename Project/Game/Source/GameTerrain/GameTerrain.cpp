#include "GameTerrain.h"
#include "Application.h"
#include "FilesManager.h"

#include "Draw/DrawEngine.h"
#include "Draw/CameraGLM.h"
#include "Object/Map.h"
#include "Object/Object.h"
#include "Common/Help.h"
#include "Common/IncludesMatem.h"

GameTerrain::GameTerrain()
{
}

GameTerrain::~GameTerrain()
{
}

void GameTerrain::init()
{
	Physics::init();

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

	if (!_visiblePhysic)
	{
        /*Object& terrainObj = _map->_objects.getByName("Plane");
        glm::mat4x4 terrainMat = terrainObj.getMatrix();
        terrainMat = glm::rotate(terrainMat, _angleMap, glm::vec3(0.0, 0.0, 1.0));
        terrainObj.setMatrix(terrainMat);*/
        
		DrawEngine::drawMap(*_map);
        
        if (_visibleVectorLight)
        {
            DrawEngine::prepareDrawLine();
            ArrayTemplate <Object>& objects = _map->_objects;

            for (int i = 0; i < objects.count(); ++i)
            {
                const glm::mat4x4& mat = objects[i].getMatrix();

                float posObject[] = { mat[3][0], mat[3][1] , mat[3][2] };
                const float* lightDirect = DrawEngine::getLightDirect();
                float lenghtLine = 10.0f;
                float lightVector[] = { (posObject[0] - lightDirect[0] * lenghtLine), (posObject[1] - lightDirect[1] * lenghtLine), (posObject[2] - lightDirect[2] * lenghtLine) };
                float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
                DrawEngine::drawLine(posObject, lightVector, color);
            }
        }
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
	DrawEngine::setAmbientColor(0.2, 0.2, 0.3);
	DrawEngine::initDrawMap();
	DrawEngine::initDrawLines();

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
	if (_charButtonUp == VK_ESCAPE)
	{
		App::close();
		return true;
	}

	if (_charButtonUp == VK_SPACE)
	{
		addObject("Dumbbell_05");
		return true;
	}

    if (_charButtonUp == 'L')
    {
        _visibleVectorLight = !_visibleVectorLight;
    }
    
	return false;
}

bool GameTerrain::pressButtonPinch(void *data)
{
	float speedCamera = 1.0f;
	if (Callback::_key[VK_SHIFT])
	{
		speedCamera = 0.125f;
	}

	if (Callback::_key['W'])
	{
		CameraGLM::current().move(CAMERA_FORVARD, speedCamera);
	}

	if (Callback::_key['S'])
	{
		CameraGLM::current().move(CAMERA_BACK, speedCamera);
	}

	if (Callback::_key['A'])
	{
		CameraGLM::current().move(CAMERA_RIGHT, speedCamera);
	}

	if (Callback::_key['D'])
	{
		CameraGLM::current().move(CAMERA_LEFT, speedCamera);
	}

	if (Callback::_key['R'])
	{
		CameraGLM::current().move(CAMERA_TOP, speedCamera);
	}

	if (Callback::_key['F'])
	{
		CameraGLM::current().move(CAMERA_DOWN, speedCamera);
	}

	return true;
}

bool GameTerrain::pressButtonDown(void *data)
{
	if (_charButtonDown == VK_SPACE)
	{
		addObject("Dumbbell_0125");
		return true;
	}


	if (Callback::_key[VK_CONTROL] && Callback::_key['P'])
	{
		_visiblePhysic = !_visiblePhysic;
	}

	return false;
}

void GameTerrain::addObject(char* name)
{
	glm::vec3 randomPos;
	randomPos.x = help::random_f(-50.0f, 100.0f);
	randomPos.y = help::random_f(-50.0f, 100.0f);
	randomPos.z = help::random_f(10.0f, 100.0f);

#ifdef BUILD_WIN_GLES
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "addObject %s [%f %f %f]\n", name, randomPos.x, randomPos.y, randomPos.z);
#endif

	_map->addObjectToPos(name, PhysicType::CONVEX, randomPos);
}
