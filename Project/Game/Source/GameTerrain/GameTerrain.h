#pragma once

#include "Game.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;

class GameTerrain : public Game, public Callback
{
private:
	Map * _map = nullptr;
	CameraGLM *_camera = nullptr;
	bool _visiblePhysic = true;

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
	bool rotateCamera(void *data);
	bool pressButton(void *data);
	bool pressButtonDown(void *data);
	void addObject(char* name);
};
