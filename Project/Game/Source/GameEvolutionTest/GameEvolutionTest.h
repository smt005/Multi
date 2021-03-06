#pragma once

#include "Game.h"
#include "Engine/Source/Physics/Physics.h"
#include "Callback/Callback.h"

#include <memory>

class Camera;

class Map;
typedef std::shared_ptr<Map> MapPtr;

class GameEvolutionTest: public Game, public Callback
{
private:
	MapPtr _map = nullptr;
	double _timerTarget = 0.0;
	int _idShape = 0;
	int _countObjects = 0;

public:
	GameEvolutionTest();
	~GameEvolutionTest();

	void init() override;
	void save() override;
	void tact() override;
	void draw() override;

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
