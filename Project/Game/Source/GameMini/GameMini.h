#pragma once

#include "Game.h"
#include "Callback/Callback.h"

class Map;
class CameraGLM;

class GameMini: public Game, public Callback
{
private:
	Map *_map = nullptr;
	CameraGLM *_camera = nullptr;

public:
	GameMini();
	~GameMini();

	void init();
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
