#pragma once

#include "Game.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;
class ShapeUnited;
class ModelUnited;
class ObjectUnited;

class GameTerrain : public Game, public Callback
{
private:
	Map * _map = nullptr;
	CameraGLM *_camera = nullptr;
	
	ShapeUnited* _shapeUnited = nullptr;
	ModelUnited* _modelUnited = nullptr;
	ObjectUnited* _objectUnited = nullptr;

public:
	GameTerrain();
	~GameTerrain();

	void init();
	void save();
	void tact();
	void draw();

	void initMap();
	void initDraw();
	void initCallback();

private:
	bool closeGame(void *data);
	bool rotateCamera(void *data);
};
