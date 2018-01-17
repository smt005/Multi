#pragma once

class Game
{
private:
	static Game *_game;

public:	
	static void gameInit();
	static void gameSave();	
	static void gameTact();
	static void gameDraw();

	void setCurrent();

	virtual void init();
	virtual void save();
	virtual void tact();
	virtual void draw();
};
