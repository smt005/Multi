#include "Game.h"
#include "GameMini/GameMini.h"
#include "GameEvolutionTest/GameEvolutionTest.h"
#include "GameTerrain/GameTerrain.h"

Game *Game::_game = nullptr;
#define GAME_CLASS GameTerrain

void Game::gameInit()
{
	GAME_CLASS *game = new GAME_CLASS();
	game->setCurrent();
	game->init();
}

void Game::gameSave()
{
	if (!_game) return;

	Game* gameTemp = _game;
	_game = nullptr;
	gameTemp->save();
	delete _game;
}

void Game::gameTact()
{
	_game->tact();
}

void Game::gameDraw()
{
	_game->draw();
}

void Game::setCurrent()
{
	if (_game)
	{
		_game->save();
		delete[] _game;
	}

	_game = this;
}

void Game::init()
{

}

void Game::save()
{
}

void Game::tact()
{

}

void Game::draw()
{

}
