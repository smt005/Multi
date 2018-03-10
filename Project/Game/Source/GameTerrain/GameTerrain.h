#pragma once

#include "Game.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;
class ShapeUnited;

class GameTerrain : public Game, public Callback
{
private:
	Map * _map = nullptr;
	CameraGLM *_camera = nullptr;
	ShapeUnited* _shapeUnited = nullptr;

	double _timerTarget = 0.0;

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
