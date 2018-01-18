#pragma once

#include "Game.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;

class GameEvolutionTest: public Game, public Callback
{
private:
	Map *_map = nullptr;
	CameraGLM *_camera = nullptr;
	double _timerTarget = 0.0;

public:
	GameEvolutionTest();
	~GameEvolutionTest();

	void init();
	void save();
	void tact();
	void draw();

	void initMap();
	void initDraw();
	void initCallback();

private:
	// Callback
	bool closeGame(void *data);
	bool rotateCamera(void *data);
};
