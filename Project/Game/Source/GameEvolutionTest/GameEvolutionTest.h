#pragma once

#include "Game.h"
#include "Engine/Source/Physics/Physics.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;

class GameEvolutionTest: public Game, public Callback
{
private:
	Map *_map = nullptr;
	CameraGLM *_camera = nullptr;
	double _timerTarget = 0.0;
	Physics physics;

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

	glm::vec3 randomPos(const float& k = 0.5, const int& zBottom = 1, const int& zTop = 1);

private:
	// Callback
	bool closeGame(void *data);
	bool rotateCamera(void *data);
	bool addObject(void *data);
};
