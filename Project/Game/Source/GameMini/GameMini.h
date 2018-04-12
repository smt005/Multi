#pragma once

#include "Game.h"
#include "Callback/Callback.h"

#include <memory>

class Camera;

class Map;
typedef std::shared_ptr<Map> MapPtr;

class GameMini: public Game, public Callback
{
private:
	MapPtr _map = nullptr;
	double _timerTarget = 0.0;

public:
	GameMini();
	~GameMini();

	void init() override;
	void save() override;
	void tact() override;
	void draw() override;

	void initMap();
	void initDraw();
	void initCallback();

private:
	// Callback
	bool closeGame(void *data);
	bool rotateCamera(void *data);
};
