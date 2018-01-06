#pragma once

class Game
{
private:
	static Game *_game;

public:
	Game();
	~Game();

	static void init();
	static void tact();
};
