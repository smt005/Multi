#pragma once

class Game
{
private:
	static Game *_game;

public:
	Game();
	~Game();

	static void gameInit();
	static void gameTact();
	static void gameDraw();

	void setCurrent();

	virtual void init();
	virtual void tact();
	virtual void draw();
};
